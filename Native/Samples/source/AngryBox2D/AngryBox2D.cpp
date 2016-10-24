#include "pch.h"
#include "AngryBox2D.h"
#include <xmllite.h>

using namespace std;
using namespace DirectX;
using namespace Library;

namespace AngryBox2DGame
{
	const XMVECTORF32 AngryBox2D::BackgroundColor = Colors::Black;

	AngryBox2D::AngryBox2D(std::function<void*()> getWindowCallback, std::function<void(SIZE&)> getRenderTargetSizeCallback) :
		Game(getWindowCallback, getRenderTargetSizeCallback), mRenderStateHelper(*this)
	{
	}

	void AngryBox2D::Initialize()
	{
		RasterizerStates::Initialize(mDirect3DDevice.Get());
		SpriteManager::Initialize(*this);

		const float viewWidth = 50.0f;
		const float viewHeight = 50.0f;
		auto camera = make_shared<OrthographicCamera>(*this, viewWidth, viewHeight);
		mComponents.push_back(camera);

		mKeyboard = make_shared<KeyboardComponent>(*this);
		mComponents.push_back(mKeyboard);
		mServices.AddService(KeyboardComponent::TypeIdClass(), mKeyboard.get());

		mMouse = make_shared<MouseComponent>(*this, MouseModes::Absolute);
		mComponents.push_back(mMouse);
		mServices.AddService(MouseComponent::TypeIdClass(), mMouse.get());

		mGamePad = make_shared<GamePadComponent>(*this);
		mComponents.push_back(mGamePad);
		mServices.AddService(GamePadComponent::TypeIdClass(), mGamePad.get());

		auto audioEngine = make_shared<AudioEngineComponent>(*this);
		mComponents.push_back(audioEngine);
		mServices.AddService(AudioEngineComponent::TypeIdClass(), audioEngine.get());

		auto physicsEngine = make_shared<Box2DComponent>(*this);
		mComponents.push_back(physicsEngine);
		mServices.AddService(Box2DComponent::TypeIdClass(), physicsEngine.get());

		auto physicDebugDraw = make_shared<Box2DDebugDraw>(*this, camera);
		mComponents.push_back(physicDebugDraw);
		mServices.AddService(Box2DDebugDraw::TypeIdClass(), physicDebugDraw.get());

		mFpsComponent = make_shared<FpsComponent>(*this);
		mFpsComponent->Initialize();

		ThrowIfFailed(WriteReaderAttributes(mXMLReader.Get()),"Error initializing xml reader");

		Game::Initialize();
		camera->SetPosition(0.0f, 0.0f, 1.0f);
	}

	void AngryBox2D::Update(const GameTime &gameTime)
	{
		if (mKeyboard->WasKeyPressedThisFrame(Keys::Escape) || mGamePad->WasButtonPressedThisFrame(GamePadButtons::Back))
		{
			Exit();
		}

		mFpsComponent->Update(gameTime);

		Game::Update(gameTime);
	}

	void AngryBox2D::Draw(const GameTime &gameTime)
	{
		mDirect3DDeviceContext->ClearRenderTargetView(mRenderTargetView.Get(), reinterpret_cast<const float*>(&BackgroundColor));
		mDirect3DDeviceContext->ClearDepthStencilView(mDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		Game::Draw(gameTime);

		mRenderStateHelper.SaveAll();
		mFpsComponent->Draw(gameTime);
		mRenderStateHelper.RestoreAll();

		HRESULT hr = mSwapChain->Present(1, 0);

		// If the device was removed either by a disconnection or a driver upgrade, we must recreate all device resources.
		if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
		{
			HandleDeviceLost();
		}
		else
		{
			ThrowIfFailed(hr, "IDXGISwapChain::Present() failed.");
		}
	}

	void AngryBox2D::Shutdown()
	{
		SpriteManager::Shutdown();
		RasterizerStates::Shutdown();

		Game::Shutdown();
	}

	void AngryBox2D::Exit()
	{
		PostQuitMessage(0);
	}
	IXmlReader * AngryBox2D::XMLReader()
	{
		return mXMLReader.Get();
	}
	HRESULT AngryBox2D::WriteReaderAttributes(IXmlReader * pReader)
	{
		const WCHAR* pwszPrefix; 
		const WCHAR* pwszLocalName; 
		const WCHAR* pwszValue; 
		HRESULT hr = pReader->MoveToFirstAttribute(); 
 
		if (S_FALSE == hr) 
			return hr; 
		if (S_OK != hr) 
		{ 
			wprintf(L"Error moving to first attribute, error is %08.8lx", hr); 
			return hr; 
		} 
		else 
		{ 
			while (TRUE) 
			{ 
				if (!pReader->IsDefault()) 
				{ 
					UINT cwchPrefix; 
					if (FAILED(hr = pReader->GetPrefix(&pwszPrefix, &cwchPrefix))) 
					{ 
						wprintf(L"Error getting prefix, error is %08.8lx", hr); 
						return hr; 
					} 
					if (FAILED(hr = pReader->GetLocalName(&pwszLocalName, NULL))) 
					{ 
						wprintf(L"Error getting local name, error is %08.8lx", hr); 
						return hr; 
					} 
					if (FAILED(hr = pReader->GetValue(&pwszValue, NULL))) 
					{ 
						wprintf(L"Error getting value, error is %08.8lx", hr); 
						return hr; 
					} 
					if (cwchPrefix > 0) 
						wprintf(L"Attr: %s:%s=\"%s\" \n", pwszPrefix, pwszLocalName, pwszValue); 
					else 
						wprintf(L"Attr: %s=\"%s\" \n", pwszLocalName, pwszValue); 
				} 
 
				if (S_OK != pReader->MoveToNextAttribute()) 
					break; 
			} 
		} 
		return hr; 
	}
}