#include "pch.h"
#include "ScoreBoard.h"
#include "BouncingLogoGame.h"
using namespace std;
using namespace Library;
using namespace DirectX;

namespace BouncingLogo {

	const XMFLOAT2 ScoreBoard::mMargin(0, 50);
	const int mScoreToWin = 10;

	ScoreBoard::ScoreBoard(Library::Game & game) : DrawableGameComponent(game), mPlayerScore(0), mComputerScore(0)
	{
	}

	void ScoreBoard::Initialize()
	{
		
		mSpriteBatch = make_unique<SpriteBatch>(mGame->Direct3DDeviceContext());
		mSpriteFont = make_unique<SpriteFont>(mGame->Direct3DDevice(), L"Content\\Fonts\\Arial_14_Regular.spritefont");

		auto& mViewport = mGame->Viewport();

		mPlayerScorePosition = XMFLOAT2(mViewport.Width * 1/4, mMargin.y);
		mComputerScorePosition = XMFLOAT2(mViewport.Width * 3/4 ,mMargin.y);
		mCenterTextPosition = XMFLOAT2(mViewport.Width * 1/10 , mViewport.Height - mMargin.y);
	}

	void ScoreBoard::Update(const Library::GameTime & gameTime)
	{
	}

	void ScoreBoard::Draw(const Library::GameTime & gameTime)
	{
		auto& mViewport = mGame->Viewport();

		mSpriteBatch->Begin();

		wostringstream playerScoreLabel;
		playerScoreLabel << setw(2) << mPlayerScore; 
		
		mSpriteFont->DrawString(mSpriteBatch.get(), playerScoreLabel.str().c_str(), mPlayerScorePosition);

		wostringstream computerScoreLabel;
		 
		computerScoreLabel << setw(2) << mComputerScore;
		mSpriteFont->DrawString(mSpriteBatch.get(), computerScoreLabel.str().c_str(), mComputerScorePosition);

		GameState currentGameState = mGame->As<BouncingLogoGame>()->GameState();
		
		wstring centertext = L"";
		if (currentGameState == GameState::Play) {
			centertext = L"Press space to pause.";
		}
		else if (currentGameState == GameState::Paused) {
			centertext = L"PAUSED Press space to continue.";
		}
		else if (currentGameState == GameState::ComputerWin) 
		{
			centertext = L"Game Over, You Lose! Press space to try again.";
		}
		else if (currentGameState == GameState::PlayerWin)
		{
			centertext = L"Congratulations, You Win! Press space to play again.";
		}
		wostringstream centerLabel;

		centerLabel << setprecision(10) << centertext;
		mSpriteFont->DrawString(mSpriteBatch.get(), centerLabel.str().c_str(), mCenterTextPosition);

		

		
		mSpriteBatch->End();

	}
	void ScoreBoard::PlayerScores()
	{
		mPlayerScore++;
		if (mPlayerScore == 10) {
			mGame->As<BouncingLogoGame>()->setGameState(GameState::PlayerWin);
		}
	}
	void ScoreBoard::ComputerScores()
	{
		mComputerScore++;
		if (mComputerScore == 10) {
			mGame->As<BouncingLogoGame>()->setGameState(GameState::ComputerWin);
		}

	}
	void ScoreBoard::ResetScores()
	{
		mPlayerScore = 0;
		mComputerScore = 0;
	}
}