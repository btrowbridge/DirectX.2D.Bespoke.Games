#include "pch.h"
#include "PongGame.h"

using namespace std;
using namespace DirectX;
using namespace Library;
using namespace Microsoft::WRL;

namespace Pong
{
	const XMVECTORF32 PongGame::BackgroundColor = Colors::Black;

	PongGame::PongGame(function<void*()> getWindowCallback, function<void(SIZE&)> getRenderTargetSizeCallback) :
		Game(getWindowCallback, getRenderTargetSizeCallback), mGameState(GameState::Play)
	{
	}

	void PongGame::Initialize()
	{
		mKeyboard = make_shared<KeyboardComponent>(*this);
		mComponents.push_back(mKeyboard);
		mServices.AddService(KeyboardComponent::TypeIdClass(), mKeyboard.get());

		mAudio = make_shared<AudioEngineComponent>(*this);
		mComponents.push_back(mAudio);
		mServices.AddService(AudioEngineComponent::TypeIdClass(), mAudio.get());

		mBall = make_shared<Ball>(*this);
		mComponents.push_back(mBall);

		mPlayer1 = make_shared<Paddle>(*this,PlayerOptions::Player1);
		mComponents.push_back(mPlayer1);

		mPlayer2 = make_shared<Paddle>(*this,PlayerOptions::Player2WithAI);
		mComponents.push_back(mPlayer2);

		mScoreBoard = make_shared<ScoreBoard>(*this);
		mComponents.push_back(mScoreBoard);

		Game::Initialize();
	}

	void PongGame::Update(const GameTime &gameTime)
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

	void PongGame::Draw(const GameTime &gameTime)
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

	void PongGame::Shutdown()
	{
	}

	void PongGame::Exit()
	{
		PostQuitMessage(0);
	}

	void PongGame::Reset()
	{
		mBall->ResetBall();
		mPlayer1->ResetPaddle();
		mPlayer2->ResetPaddle();
		mScoreBoard->ResetScores();
		setGameState(GameState::Play);
	}

	void PongGame::setGameState(Pong::GameState state)
	{
		mGameState = state;
	}

	Pong::GameState & PongGame::GameState()
	{
		return mGameState;
	}

	Ball * PongGame::getBall()
	{
		return mBall.get();
	}

	Paddle * PongGame::getPlayer1()
	{
		return mPlayer1.get();
	}

	Paddle * PongGame::getPlayer2()
	{
		return mPlayer2.get();
	}

	ScoreBoard * PongGame::getScoreBoard()
	{
		return mScoreBoard.get();
	}
}