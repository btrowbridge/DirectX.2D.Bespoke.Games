#include "pch.h"
#include "SimpleCollisionGame.h"

using namespace std;
using namespace DirectX;
using namespace Library;
using namespace Microsoft::WRL;

namespace SimpleCollision
{
	const XMVECTORF32 SimpleCollisionGame::BackgroundColor = Colors::Black;

	SimpleCollisionGame::SimpleCollisionGame(function<void*()> getWindowCallback, function<void(SIZE&)> getRenderTargetSizeCallback) :
		Game(getWindowCallback, getRenderTargetSizeCallback)
	{
	}

	void SimpleCollisionGame::Initialize()
	{

		mKeyboard = make_shared<KeyboardComponent>(*this);
		mComponents.push_back(mKeyboard);
		mServices.AddService(KeyboardComponent::TypeIdClass(), mKeyboard.get());

		mAudio = make_shared<AudioEngineComponent>(*this);
		mComponents.push_back(mAudio);
		mServices.AddService(AudioEngineComponent::TypeIdClass(), mAudio.get());

		
		SpriteManager::Initialize(*this);

		Game::Initialize();
	}

	void SimpleCollisionGame::Update(const GameTime &gameTime)
	{
		if (mKeyboard->WasKeyPressedThisFrame(Keys::Escape))
		{
			Exit();
		}

		Game::Update(gameTime);
	}

	void SimpleCollisionGame::Draw(const GameTime &gameTime)
	{
		mDirect3DDeviceContext->ClearRenderTargetView(mRenderTargetView.Get(), reinterpret_cast<const float*>(&BackgroundColor));
		mDirect3DDeviceContext->ClearDepthStencilView(mDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		Game::Draw(gameTime);

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

	void SimpleCollisionGame::Shutdown()
	{
	}

	void SimpleCollisionGame::Exit()
	{
		PostQuitMessage(0);
	}
}