#include "pch.h"
#include "PlayerPaddle.h"
#include "SpriteBatch.h"

using namespace Library;
using namespace std;
using namespace Microsoft::WRL;
using namespace DirectX;

namespace BouncingLogo {
	const int BouncingLogo::PlayerPaddle::mSpeed = 10;

	PlayerPaddle::PlayerPaddle(Library::Game & game, std::shared_ptr<Library::KeyboardComponent> keyboard) : Paddle(game), mKeyboard(keyboard)
	{
	}

	void PlayerPaddle::Initialize()
	{
		mVelocity.x = 0;
		mVelocity.y = 0;
		
		mBounds.X = mTextureHalfSize.X;
		mBounds.Y = mGame->Viewport().Height / 2;
	}

	void PlayerPaddle::Update(const Library::GameTime & gameTime)
	{
		float elapsedTime = gameTime.ElapsedGameTimeSeconds().count();

		if (mKeyboard->IsKeyDown(Keys::Up) && mBounds.Y - mTextureHalfSize.Y - mVelocity.y > 0)
		{
			mVelocity.y = mSpeed * elapsedTime;
		}
		else if (mKeyboard->IsKeyDown(Keys::Down) && mBounds.Y - mTextureHalfSize.Y - mVelocity.y < mGame->Viewport().Height)
		{
			mVelocity.y = -mSpeed * elapsedTime;
		}
	}
	

}