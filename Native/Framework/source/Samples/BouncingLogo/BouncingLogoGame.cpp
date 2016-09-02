#include "pch.h"
#include "BouncingLogoGame.h"

using namespace std;
using namespace DirectX;
using namespace Library;
using namespace Microsoft::WRL;

namespace BouncingLogo
{
	const XMVECTORF32 BouncingLogoGame::BackgroundColor = Colors::Black;

	BouncingLogoGame::BouncingLogoGame(function<void*()> getWindowCallback, function<void(SIZE&)> getRenderTargetSizeCallback) :
		Game(getWindowCallback, getRenderTargetSizeCallback)
	{
	}

	void BouncingLogoGame::Initialize()
	{
		mKeyboard = make_shared<KeyboardComponent>(*this);
		mComponents.push_back(mKeyboard);
		mServices.AddService(KeyboardComponent::TypeIdClass(), mKeyboard.get());

		mBall = make_shared<Ball>(*this);
		mComponents.push_back(mBall);

		mPlayer = make_shared<PlayerPaddle>(*this,*mKeyboard);
		mComponents.push_back(mPlayer);
		
		mComPaddle = make_shared<ComputerPaddle>(*this, *mBall);
		mComponents.push_back(mComPaddle);

		Game::Initialize();
	}

	void BouncingLogoGame::Update(const GameTime &gameTime)
	{
		if (mKeyboard->WasKeyPressedThisFrame(Keys::Escape))
		{
			Exit();
		}

		Game::Update(gameTime);
	}

	void BouncingLogoGame::Draw(const GameTime &gameTime)
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

	void BouncingLogoGame::Shutdown()
	{
	
	}

	PlayerPaddle& BouncingLogoGame::Player()
	{
		return *mPlayer;
	}

	ComputerPaddle& BouncingLogoGame::Computer()
	{
		return *mComPaddle;
	}

	void BouncingLogoGame::Exit()
	{
		PostQuitMessage(0);
	}
}