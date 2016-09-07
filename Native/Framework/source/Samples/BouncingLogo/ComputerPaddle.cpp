#include "pch.h"
#include "ComputerPaddle.h"
#include "SpriteBatch.h"
#include "BouncingLogoGame.h"

using namespace Library;
using namespace std;
using namespace Microsoft::WRL;
using namespace DirectX;

namespace BouncingLogo {
	const int BouncingLogo::ComputerPaddle::mSpeed = 3000;

	ComputerPaddle::ComputerPaddle(Library::Game & game) : Paddle(game)
	{
	}

	void ComputerPaddle::Initialize()
	{
		Paddle::Initialize();

		Paddle::mDefaultPosition.X = static_cast<int>(mGame->Viewport().Width - 2 * mBounds.Width);
		Paddle::mDefaultPosition.Y = static_cast<int>((mGame->Viewport().Height / 2) - mBounds.Height);

		ResetPaddle();
	}

	void ComputerPaddle::Update(const Library::GameTime & gameTime)
	{
		Paddle::Update(gameTime);
		if (mGame->As<BouncingLogoGame>()->GameState() == GameState::Play) {
			float elapsedTime = gameTime.ElapsedGameTimeSeconds().count();
			auto& mViewport = mGame->Viewport();
			mBall = mGame->As<BouncingLogoGame>()->getBall();

			XMFLOAT2 positionDelta(mVelocity.x * elapsedTime, mVelocity.y * elapsedTime);

			int dY = static_cast<int>(std::round(positionDelta.y));

			if ((mBounds.Y + mBounds.Height + dY < mViewport.Height) && (mBounds.Y + dY > 0))
			{
				mBounds.Y += dY;
			}

			int distanceY = mBall->Position().Y - mBounds.Center().Y;

			if (mBall->Position().Y < mBounds.Y + .5 * mBounds.Height || mBall->Position().Y > mBounds.Y + .5 * mBounds.Height)
				mVelocity.y = distanceY / mViewport.Height * mSpeed;
		}
	}
}