#include "pch.h"
#include "Box2DDemoGame.h"
#include "PhysicsDemo.h"

using namespace std;
using namespace DirectX;
using namespace Library;

namespace Box2DDemo
{
	const XMVECTORF32 Box2DDemoGame::BackgroundColor = Colors::Black;

	Box2DDemoGame::Box2DDemoGame(std::function<void*()> getWindowCallback, std::function<void(SIZE&)> getRenderTargetSizeCallback) :
		Game(getWindowCallback, getRenderTargetSizeCallback), mRenderStateHelper(*this)
	{
	}

	void Box2DDemoGame::Initialize()
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

		auto demo = make_shared<PhysicsDemo>(*this, camera);
		mComponents.push_back(demo);

		Game::Initialize();
		camera->SetPosition(0.0f, 0.0f, 1.0f);
	}

	void Box2DDemoGame::Update(const GameTime &gameTime)
	{
		if (mKeyboard->WasKeyPressedThisFrame(Keys::Escape) || mGamePad->WasButtonPressedThisFrame(GamePadButtons::Back))
		{
			Exit();
		}

		mFpsComponent->Update(gameTime);

		Game::Update(gameTime);
	}

	void Box2DDemoGame::Draw(const GameTime &gameTime)
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

	void Box2DDemoGame::Shutdown()
	{
		SpriteManager::Shutdown();
		RasterizerStates::Shutdown();

		Game::Shutdown();
	}

	void Box2DDemoGame::Exit()
	{
		PostQuitMessage(0);
	}
}