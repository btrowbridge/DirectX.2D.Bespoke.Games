#include "pch.h"
#include "ScoreBoard.h"
#include "BouncingLogoGame.h"
using namespace std;
using namespace Library;
using namespace DirectX;

namespace BouncingLogo {

	const XMFLOAT2 ScoreBoard::mMargin(100, 50);

	ScoreBoard::ScoreBoard(Library::Game & game) : DrawableGameComponent(game), mPlayerScore(0), mComputerScore(0)
	{
	}

	void ScoreBoard::Initialize()
	{

		mSpriteBatch = make_unique<SpriteBatch>(mGame->Direct3DDeviceContext());
		mSpriteFont = make_unique<SpriteFont>(mGame->Direct3DDevice(), L"Content\\Fonts\\Arial_14_Regular.spritefont");


		auto& mViewport = mGame->Viewport();


		mPlayerScorePosition = XMFLOAT2(mViewport.TopLeftX*.25 + mMargin.x,mMargin.y);
		mComputerScorePosition = XMFLOAT2(mViewport.Width*.75, mMargin.y);
	}

	void ScoreBoard::Update(const Library::GameTime & gameTime)
	{
	}

	void ScoreBoard::Draw(const Library::GameTime & gameTime)
	{
		auto& mViewport = mGame->Viewport();

		mSpriteBatch->Begin();

		wostringstream playerScoreLabel;
		playerScoreLabel << setw(mViewport.Width) << mPlayerScore;
		
		mSpriteFont->DrawString(mSpriteBatch.get(), playerScoreLabel.str().c_str(), mPlayerScorePosition);

		wostringstream computerScoreLabel;

		computerScoreLabel << setw(mViewport.Width) << mComputerScore;
		mSpriteFont->DrawString(mSpriteBatch.get(), computerScoreLabel.str().c_str(), mComputerScorePosition);

		mSpriteBatch->End();

	}
}