#include "pch.h"
#include "PlayerPaddle.h"
#include "SpriteBatch.h"

using namespace Library;
using namespace std;
using namespace Microsoft::WRL;
using namespace DirectX;

namespace BouncingLogo {
	const int BouncingLogo::PlayerPaddle::mSpeed = 400;

	PlayerPaddle::PlayerPaddle(Library::Game & game) : Paddle(game)
	{
	}

	void PlayerPaddle::Initialize()
	{
		Paddle::Initialize();

		mDefaultPosition.X = mBounds.Width;
		mDefaultPosition.Y = static_cast<int>((mGame->Viewport().Height / 2) - (float)mBounds.Height);

		ResetPaddle();

		mKeyboard = reinterpret_cast<KeyboardComponent*>(mGame->Services().GetService(Library::KeyboardComponent::TypeIdClass()));

		assert(mKeyboard != nullptr);
	}

	void PlayerPaddle::Update(const Library::GameTime & gameTime)
	{
		Paddle::Update(gameTime);
		if (mGame->As<BouncingLogoGame>()->GameState() == GameState::Play) {
			float elapsedTime = gameTime.ElapsedGameTimeSeconds().count();
			auto& mViewport = mGame->Viewport();

			XMFLOAT2 positionDelta(mVelocity.x * elapsedTime, mVelocity.y * elapsedTime);
			int dY = static_cast<int>(std::round(positionDelta.y));

			if ((mBounds.Y + mBounds.Height + dY < mViewport.Height) && (mBounds.Y + dY > 0))
			{
				mBounds.Y += dY;
			}
			if (mKeyboard->IsKeyDown(Keys::Up))
			{
				mVelocity.y = -mSpeed;
			}
			else if (mKeyboard->IsKeyDown(Keys::Down))
			{
				mVelocity.y = mSpeed;
			}
			else
			{
				mVelocity.y = 0;
			}


			
		}
	}
}