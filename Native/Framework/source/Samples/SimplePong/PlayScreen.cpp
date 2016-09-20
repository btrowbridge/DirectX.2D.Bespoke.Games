#include "pch.h"
#include "PlayScreen.h"

using namespace std;
using namespace Library;

namespace Pong {

	PlayScreen::PlayScreen(std::shared_ptr<Library::ScreenManager> screenManager) :
		GameScreen(screenManager), mGameState(Pong::GameState::Play)
	{

	}

	void PlayScreen::Initialize()
	{
		
		mBall = make_shared<Ball>(*this);
		mComponents.push_back(mBall);

		mPlayer1 = make_shared<Paddle>(*this,PlayerOptions::Player1);
		mComponents.push_back(mPlayer1);

		mPlayer2 = make_shared<Paddle>(*this,PlayerOptions::Player2WithAI);
		mComponents.push_back(mPlayer2);

		mScoreBoard = make_shared<ScoreBoard>(*this);
		mComponents.push_back(mScoreBoard);
		
		

		GameScreen::Initialize();
	}

	void PlayScreen::OnExit()
	{
		GameScreen::OnExit();
	}

	void PlayScreen::Draw(const Library::GameTime & gameTime)
	{
		GameScreen::Draw(gameTime);
	}

	void PlayScreen::Update(const Library::GameTime & gameTime, bool otherScreenHasFocus, bool coveredByOtherScreen)
	{
		if (mKeyboard->WasKeyPressedThisFrame(Keys::Space))
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
	}

	void PlayScreen::HandleInput(Library::ScreenInputManager & inputManager)
	{
		GameScreen::HandleInput(inputManager);
	}

	void PlayScreen::Reset()
	{
		mBall->ResetBall();
		mPlayer1->ResetPaddle();
		mPlayer2->ResetPaddle();
		mScoreBoard->ResetScores();
		setGameState(GameState::Play);
	}

	void PlayScreen::setGameState(Pong::GameState state)
	{
		mGameState = state;
	}

	GameState & PlayScreen::GameState()
	{
		return mGameState;
	}

}