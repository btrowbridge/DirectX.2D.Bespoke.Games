#include "pch.h"
#include "ComputerPaddle.h"
#include "SpriteBatch.h"

using namespace Library;
using namespace std;
using namespace Microsoft::WRL;
using namespace DirectX;

namespace BouncingLogo {
	const int BouncingLogo::ComputerPaddle::mSpeed = 2000;

	ComputerPaddle::ComputerPaddle(Library::Game & game, Ball & ball) : Paddle(game), mBall(&ball)
	{
	}

	void ComputerPaddle::Initialize()
	{
		Paddle::Initialize();
		mVelocity.x = 0;
		mVelocity.y = 0;

		mBounds.X = mGame->Viewport().Width - 2 * mBounds.Width;
		mBounds.Y = (mGame->Viewport().Height / 2) - mBounds.Height;
	}

	void ComputerPaddle::Update(const Library::GameTime & gameTime)
	{
		Paddle::Update(gameTime);

		float elapsedTime = gameTime.ElapsedGameTimeSeconds().count();
		auto& mViewport = mGame->Viewport();

		XMFLOAT2 positionDelta(mVelocity.x * elapsedTime, mVelocity.y * elapsedTime);

		int dY = static_cast<int>(std::round(positionDelta.y));
	    

		if ((mBounds.Y + mBounds.Height + dY < mViewport.Height) && (mBounds.Y + dY > 0))
		{
			mBounds.Y += dY;
		}

		int distanceY = mBall->Position().Y - mBounds.Center().Y;
		
		if(mBall->Position().Y < mBounds.Y + .5 * mBounds.Height || mBall->Position().Y > mBounds.Y + .5 * mBounds.Height)
			mVelocity.y =  distanceY/mViewport.Height * mSpeed;


	}


}