#include "pch.h"
#include "ScoreBoard.h"
#include "PongGame.h"
using namespace std;
using namespace Library;
using namespace DirectX;

namespace Pong {
	const XMFLOAT2 ScoreBoard::mMargin(100, 30);

	ScoreBoard::ScoreBoard(Library::Game & game, PlayScreen* screen) :
		DrawableGameComponent(game), mPlayer1Score(0), mPlayer2Score(0), mScreen(screen), mScoreToWin(5)
	{
	}

	void ScoreBoard::Initialize()
	{
		mSpriteBatch = make_unique<SpriteBatch>(mGame->Direct3DDeviceContext());

		mViewport = mGame->Viewport();

		mPlayer1ScorePosition = XMFLOAT2(mViewport.Width * 1 / 4, mMargin.y);
		mPlayer2ScorePosition = XMFLOAT2(mViewport.Width * 3 / 4, mMargin.y);

		mSpriteFont = make_unique<SpriteFont>(mGame->Direct3DDevice(), L"Content\\Fonts\\Arial_14_Regular.spritefont");
		mPlayer1ScorePosition = XMFLOAT2(static_cast<float>(mMargin.x), static_cast<float>(mMargin.y));
		mPlayer2ScorePosition = XMFLOAT2(mViewport.Width - static_cast<float>(mMargin.x), static_cast<float>(mMargin.y));

		DrawableGameComponent::Initialize();
	}

	void ScoreBoard::Update(const Library::GameTime & gameTime)
	{
		DrawableGameComponent::Update(gameTime);
	}

	void ScoreBoard::Draw(const Library::GameTime & gameTime)
	{
		mSpriteBatch->Begin();

		mSpriteFont->DrawString(mSpriteBatch.get(), to_wstring(mPlayer1Score).c_str(), mPlayer1ScorePosition);
		mSpriteFont->DrawString(mSpriteBatch.get(), to_wstring(mPlayer2Score).c_str(), mPlayer2ScorePosition);

		GameState currentGameState = mScreen->getGameState();

		wstring centertext = L"";
		if (currentGameState == GameState::Play) {
			centertext = L"Press space to pause.";
		}
		else if (currentGameState == GameState::Paused) {
			centertext = L"PAUSED Press space to continue. Backspace to return to Menu.";
		}
		else if (currentGameState == GameState::Player2Win)
		{
			centertext = L"Player 2 Wins! Press space to try again. Backspace to return to Menu.";
		}
		else if (currentGameState == GameState::Player1Win)
		{
			centertext = L"Player 1 Wins! Press space to play again. Backspace to return to Menu.";
		}

		XMFLOAT2 tempViewportSize(mViewport.Width, mViewport.Height);
		XMVECTOR viewportSize = XMLoadFloat2(&tempViewportSize);

		XMVECTOR messageSize = mSpriteFont->MeasureString(centertext.c_str());

		XMStoreFloat2(&mCenterTextPosition, (viewportSize - messageSize) / 2);
		mCenterTextPosition.y += XMVectorGetY(messageSize) + (mViewport.Height / 4);

		mSpriteFont->DrawString(mSpriteBatch.get(), centertext.c_str(), mCenterTextPosition);

		mSpriteBatch->End();

		DrawableGameComponent::Draw(gameTime);
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
		UpdateScorePositions(ScorePosition::ScorePositionLeft);
		if (mPlayer1Score == mScoreToWin) {
			mScreen->setGameState(GameState::Player1Win);
		}
	}
	void ScoreBoard::Player2Scores()
	{
		mPlayer2Score++;
		UpdateScorePositions(ScorePosition::ScorePositionRight);
		if (mPlayer2Score == mScoreToWin) {
			mScreen->setGameState(GameState::Player2Win);
		}
	}
	void ScoreBoard::ResetScores()
	{
		mPlayer1Score = 0;
		mPlayer2Score = 0;

		UpdateScorePositions(ScorePosition::ScorePositionBoth);
	}
}