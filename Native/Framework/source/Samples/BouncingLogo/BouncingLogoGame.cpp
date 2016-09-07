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
		Game(getWindowCallback, getRenderTargetSizeCallback), mGameState(GameState::Play)
	{
	}

	void BouncingLogoGame::Initialize()
	{
		mKeyboard = make_shared<KeyboardComponent>(*this);
		mComponents.push_back(mKeyboard);
		mServices.AddService(KeyboardComponent::TypeIdClass(), mKeyboard.get());

		mAudio = make_shared<AudioEngineComponent>(*this);
		mComponents.push_back(mAudio);
		mServices.AddService(AudioEngineComponent::TypeIdClass(), mAudio.get());

		mBall = make_shared<Ball>(*this);
		mComponents.push_back(mBall);

		mPlayer = make_shared<PlayerPaddle>(*this);
		mComponents.push_back(mPlayer);

		mComPaddle = make_shared<ComputerPaddle>(*this);
		mComponents.push_back(mComPaddle);

		mScoreBoard = make_shared<ScoreBoard>(*this);
		mComponents.push_back(mScoreBoard);

		Game::Initialize();
	}

	void BouncingLogoGame::Update(const GameTime &gameTime)
	{
		if (mKeyboard->WasKeyPressedThisFrame(Keys::Escape))
		{
			Exit();
		}
		else if (mKeyboard->WasKeyPressedThisFrame(Keys::Space))
		{
			if (mGameState == GameState::Play)
			{
				setGameState(GameState::Paused);
			}
			else if (mGameState == GameState::Paused)
			{
				setGameState(GameState::Play);
			}
			else
			{
				Reset();
			}
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

	void BouncingLogoGame::Exit()
	{
		PostQuitMessage(0);
	}

	void BouncingLogoGame::Reset()
	{
		mBall->ResetBall();
		mPlayer->ResetPaddle();
		mComPaddle->ResetPaddle();
		mScoreBoard->ResetScores();
		setGameState(GameState::Play);
	}

	void BouncingLogoGame::setGameState(BouncingLogo::GameState state)
	{
		mGameState = state;
	}

	BouncingLogo::GameState & BouncingLogoGame::GameState()
	{
		return mGameState;
	}

	Ball * BouncingLogoGame::getBall()
	{
		return mBall.get();
	}

	PlayerPaddle * BouncingLogoGame::getPlayer()
	{
		return mPlayer.get();
	}

	ComputerPaddle * BouncingLogoGame::getComputer()
	{
		return mComPaddle.get();
	}

	ScoreBoard * BouncingLogoGame::getScoreBoard()
	{
		return mScoreBoard.get();
	}
}