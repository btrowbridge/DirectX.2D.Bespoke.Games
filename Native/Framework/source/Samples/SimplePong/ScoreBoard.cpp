#include "pch.h"
#include "ScoreBoard.h"
#include "PongGame.h"
using namespace std;
using namespace Library;
using namespace DirectX;

namespace Pong {

	const XMFLOAT2 ScoreBoard::mMargin(100, 30);
	const int mScoreToWin = 10;

	ScoreBoard::ScoreBoard(Library::Game & game) : DrawableGameComponent(game), mPlayer1Score(0), mPlayer2Score(0)
	{
	}

	void ScoreBoard::Initialize()
	{
		
		mSpriteBatch = make_unique<SpriteBatch>(mGame->Direct3DDeviceContext());
		//mSpriteFont = make_unique<SpriteFont>(mGame->Direct3DDevice(), L"Content\\Fonts\\Arial_14_Regular.spritefont");

		mViewport = mGame->Viewport();

		mPlayer1ScorePosition = XMFLOAT2(mViewport.Width * 1/4, mMargin.y);
		mPlayer2ScorePosition = XMFLOAT2(mViewport.Width * 3/4 ,mMargin.y);
		mCenterTextPosition = XMFLOAT2(mViewport.Width * 1/10 , mViewport.Height - mMargin.y);

		mSpriteFont = make_unique<SpriteFont>(mGame->Direct3DDevice(), L"Content\\Fonts\\Arial_36_Regular.spritefont");
		mPlayer1ScorePosition = XMFLOAT2(static_cast<float>(mMargin.x), static_cast<float>(mMargin.y));
		mPlayer2ScorePosition = XMFLOAT2(mViewport.Width - static_cast<float>(mMargin.x), static_cast<float>(mMargin.y));


	}

	void ScoreBoard::Update(const Library::GameTime & gameTime)
	{
		UNREFERENCED_PARAMETER(gameTime);
	}

	void ScoreBoard::Draw(const Library::GameTime & gameTime)
	{
		UNREFERENCED_PARAMETER(gameTime);

		mSpriteBatch->Begin();

		mSpriteFont->DrawString(mSpriteBatch.get(), to_wstring(mPlayer1Score).c_str(), mPlayer1ScorePosition);
		mSpriteFont->DrawString(mSpriteBatch.get(), to_wstring(mPlayer2Score).c_str(), mPlayer2ScorePosition);

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

		XMFLOAT2 tempViewportSize(mViewport.Width, mViewport.Height);
		XMVECTOR viewportSize= XMLoadFloat2(&tempViewportSize);

		XMVECTOR messageSize = mSpriteFont->MeasureString(centertext.c_str());


		XMStoreFloat2(&mCenterTextPosition, (viewportSize - messageSize) / 2);
		mCenterTextPosition.y -= XMVectorGetY(messageSize);

		mSpriteFont->DrawString(mSpriteBatch.get(), centertext.c_str(), mCenterTextPosition);

		mSpriteBatch->End();

	}
	void ScoreBoard::UpdateScorePositions(ScorePosition positionToUpdate)
	{
		if (positionToUpdate & ScorePositionLeft)
		{
			wstring scoreMessage = to_wstring(mPlayer1Score);
			XMVECTOR scoreWidth = mSpriteFont->MeasureString(scoreMessage.c_str());
			mPlayer1ScorePosition.x = static_cast<float>(mMargin.x) - XMVectorGetX(scoreWidth);
		}

		if (positionToUpdate & ScorePositionRight)
		{
			wstring scoreMessage = to_wstring(mPlayer2Score);
			XMVECTOR scoreWidth = mSpriteFont->MeasureString(scoreMessage.c_str());
			mPlayer2ScorePosition.x = static_cast<float>(mGame->Viewport().Width) - mMargin.x - XMVectorGetX(scoreWidth);
		}
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