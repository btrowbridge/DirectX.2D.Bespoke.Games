#include "pch.h"
#include "Reflection2D.h"
#include <xmllite.h>

using namespace std;
using namespace DirectX;
using namespace Library;

namespace Reflection2DGame
{
	const XMVECTORF32 Reflection2D::BackgroundColor = Colors::Black;

	Reflection2D::Reflection2D(std::function<void*()> getWindowCallback, std::function<void(SIZE&)> getRenderTargetSizeCallback) :
		Game(getWindowCallback, getRenderTargetSizeCallback), mRenderStateHelper(*this)
	{
	}

	void Reflection2D::Initialize()
	{
		SamplerStates::Initialize(mDirect3DDevice.Get());
		BlendStates::Initialize(mDirect3DDevice.Get());
		RasterizerStates::Initialize(mDirect3DDevice.Get());
		SpriteManager::Initialize(*this);
		

		const float viewWidth = 50.0f;
		const float viewHeight = 50.0f;
		auto camera = make_shared<OrthographicCamera>(*this, viewWidth, viewHeight);
		mComponents.push_back(camera);
		mServices.AddService(Camera::TypeIdClass(), camera.get());

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
		
		auto demo = make_shared<ReflectionDemo>(*this, camera);
		mComponents.push_back(demo);

		auto physicDebugDraw = make_shared<Box2DDebugDraw>(*this, camera);
		mComponents.push_back(physicDebugDraw);
		mServices.AddService(Box2DDebugDraw::TypeIdClass(), physicDebugDraw.get());

		mFpsComponent = make_shared<FpsComponent>(*this);
		mFpsComponent->Initialize();

		Game::Initialize();
		camera->SetPosition(0.0f, 0.0f, 10.0f);
	}

	void Reflection2D::Update(const GameTime &gameTime)
	{
		if (mKeyboard->WasKeyPressedThisFrame(Keys::Escape) || mGamePad->WasButtonPressedThisFrame(GamePadButtons::Back))
		{
			Exit();
		}

		mFpsComponent->Update(gameTime);

		Game::Update(gameTime);
	}

	void Reflection2D::Draw(const GameTime &gameTime)
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

	void Reflection2D::Shutdown()
	{
		SpriteManager::Shutdown();
		RasterizerStates::Shutdown();
		BlendStates::Shutdown();
		SamplerStates::Shutdown();
		

		Game::Shutdown();
	}

	void Reflection2D::Exit()
	{
		PostQuitMessage(0);
	}
}