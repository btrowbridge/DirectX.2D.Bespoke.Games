#include "pch.h"
#include "ScoreBoard.h"
#include "PongGame.h"
using namespace std;
using namespace Library;
using namespace DirectX;

namespace Pong {

	const XMFLOAT2 ScoreBoard::mMargin(0, 50);
	const int mScoreToWin = 10;

	ScoreBoard::ScoreBoard(Library::Game & game) : DrawableGameComponent(game), mPlayer1Score(0), mPlayer2Score(0)
	{
	}

	void ScoreBoard::Initialize()
	{
		
		mSpriteBatch = make_unique<SpriteBatch>(mGame->Direct3DDeviceContext());
		mSpriteFont = make_unique<SpriteFont>(mGame->Direct3DDevice(), L"Content\\Fonts\\Arial_14_Regular.spritefont");

		auto& mViewport = mGame->Viewport();

		mPlayer1ScorePosition = XMFLOAT2(mViewport.Width * 1/4, mMargin.y);
		mPlayer2ScorePosition = XMFLOAT2(mViewport.Width * 3/4 ,mMargin.y);
		mCenterTextPosition = XMFLOAT2(mViewport.Width * 1/10 , mViewport.Height - mMargin.y);
	}

	void ScoreBoard::Update(const Library::GameTime & gameTime)
	{
		UNREFERENCED_PARAMETER(gameTime);
	}

	void ScoreBoard::Draw(const Library::GameTime & gameTime)
	{
		UNREFERENCED_PARAMETER(gameTime);

		mSpriteBatch->Begin();

		wostringstream Player1ScoreLabel;
		Player1ScoreLabel << setw(2) << mPlayer1Score; 
		
		mSpriteFont->DrawString(mSpriteBatch.get(), Player1ScoreLabel.str().c_str(), mPlayer1ScorePosition);

		wostringstream Player2ScoreLabel;
		 
		Player2ScoreLabel << setw(2) << mPlayer2Score;
		mSpriteFont->DrawString(mSpriteBatch.get(), Player2ScoreLabel.str().c_str(), mPlayer2ScorePosition);

		GameState currentGameState = mGame->As<PongGame>()->GameState();
		
		wstring centertext = L"";
		if (currentGameState == GameState::Play) {
			centertext = L"Press space to pause.";
		}
		else if (currentGameState == GameState::Paused) {
			centertext = L"PAUSED Press space to continue.";
		}
		else if (currentGameState == GameState::Player2Win) 
		{
			centertext = L"Game Over, You Lose! Press space to try again.";
		}
		else if (currentGameState == GameState::Player1Win)
		{
			centertext = L"Congratulations, You Win! Press space to play again.";
		}
		wostringstream centerLabel;

		centerLabel << setprecision(10) << centertext;
		mSpriteFont->DrawString(mSpriteBatch.get(), centerLabel.str().c_str(), mCenterTextPosition);

		

		
		mSpriteBatch->End();

	}
	void ScoreBoard::Player1Scores()
	{
		mPlayer1Score++;
		if (mPlayer1Score == 10) {
			mGame->As<PongGame>()->setGameState(GameState::Player1Win);
		}
	}
	void ScoreBoard::Player2Scores()
	{
		mPlayer2Score++;
		if (mPlayer2Score == 10) {
			mGame->As<PongGame>()->setGameState(GameState::Player2Win);
		}

	}
	void ScoreBoard::ResetScores()
	{
		mPlayer1Score = 0;
		mPlayer2Score = 0;
	}
}