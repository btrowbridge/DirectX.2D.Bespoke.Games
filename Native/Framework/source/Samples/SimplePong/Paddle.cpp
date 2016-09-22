#include "pch.h"
#include "Paddle.h"
#include "SpriteBatch.h"

using namespace Library;
using namespace std;
using namespace Microsoft::WRL;
using namespace DirectX;

namespace Pong {

	const int Paddle::mSpeed = 500;
	const int Paddle::mAISpeedMultiplier = 4;

	Pong::Paddle::Paddle(Library::Game& game, PlayScreen* screen, PlayerOptions playerOption) 
		: DrawableGameComponent(game), mBounds(Rectangle::Empty), mPlayerOption(playerOption), mScreen(screen)
	{
	}

	void Pong::Paddle::Initialize()
	{
		// Load a texture
		ComPtr<ID3D11Resource> textureResource;
		wstring textureName = L"Content\\Textures\\Paddle.png";

		ThrowIfFailed(CreateWICTextureFromFile(mGame->Direct3DDevice(), textureName.c_str(),
			textureResource.ReleaseAndGetAddressOf(), mTexture.ReleaseAndGetAddressOf()),
			"CreateWICTextureFromFile() Failed.");

		ComPtr<ID3D11Texture2D> texture;
		ThrowIfFailed(textureResource.As(&texture), "Invalid ID3D11 texture resource");

		mBounds = TextureHelper::GetTextureBounds(texture.Get());

		mSpriteBatch = make_unique<SpriteBatch>(mGame->Direct3DDeviceContext());

		mDefaultPosition.Y = static_cast<int>((mGame->Viewport().Height / 2) - (float)mBounds.Height/2);

		if (mPlayerOption & PlayerOptions::Player1)
		{
			mUpKey = Keys::W;
			mDownKey = Keys::S;

			mDefaultPosition.X = mBounds.Width;
		}
		else 
		{
			mUpKey = Keys::Up;
			mDownKey = Keys::Down;
			mDefaultPosition.X = static_cast<int>(mGame->Viewport().Width - 2 * mBounds.Width);
		}

		isAIEnabled = (mPlayerOption & PlayerOptions::AI) ? true : false;

		ResetPaddle();

		mKeyboard = reinterpret_cast<KeyboardComponent*>(mGame->Services().GetService(Library::KeyboardComponent::TypeIdClass()));

		assert(mKeyboard != nullptr);

		mBall = mScreen->getBall();

		DrawableGameComponent::Initialize();

	}

	void Pong::Paddle::Update(const Library::GameTime & gameTime)
	{
		if (mScreen->getGameState() == GameState::Play) {
			float elapsedTime = gameTime.ElapsedGameTimeSeconds().count();
			auto& mViewport = mGame->Viewport();

			XMFLOAT2 positionDelta(mVelocity.x * elapsedTime, mVelocity.y * elapsedTime);
			int dY = static_cast<int>(std::round(positionDelta.y));

			if (isAIEnabled) {
				if ((mBounds.Y + mBounds.Height + dY < mViewport.Height) && (mBounds.Y + dY > 0))
				{
					mBounds.Y += dY;
				}

				int distanceY;
				if ((mBall->Position().X > mViewport.Width/2 && (mPlayerOption & PlayerOptions::Player1)) ||
					(mBall->Position().X <  mViewport.Width/2 && (mPlayerOption & PlayerOptions::Player2))) {
					distanceY = mDefaultPosition.Y - mBounds.Center().Y;
				}
				else {
					distanceY = mBall->Position().Y - mBounds.Center().Y;
					distanceY += 2 * mBounds.Height * (distanceY > 0) ? 1:-1;
					
				}
				
				mVelocity.y = mAISpeedMultiplier * distanceY/ mViewport.Height * mSpeed;
			}
			else {
				if ((mBounds.Y + mBounds.Height + dY < mViewport.Height) && (mBounds.Y + dY > 0))
				{
					mBounds.Y += dY;
				}
				if (mKeyboard->IsKeyDown(mUpKey))
				{
					mVelocity.y = static_cast<float>(-mSpeed);
				}
				else if (mKeyboard->IsKeyDown(mDownKey))
				{
					mVelocity.y = static_cast<float>(mSpeed);
				}
				else
				{
					mVelocity.y = 0;
				}
			}
		}
		DrawableGameComponent::Update(gameTime);
	}

	void Pong::Paddle::Draw(const Library::GameTime & gameTime)
	{
	

		XMFLOAT2 position((float)mBounds.X, (float)mBounds.Y);

		mSpriteBatch->Begin();
		mSpriteBatch->Draw(mTexture.Get(), position);
		mSpriteBatch->End();


		DrawableGameComponent::Draw(gameTime);
	}

	void Paddle::ResetPaddle()
	{
		mVelocity = XMFLOAT2();
		mBounds.X = mDefaultPosition.X;
		mBounds.Y = mDefaultPosition.Y;
	}

	Library::Rectangle Pong::Paddle::Bounds()
	{
		return mBounds;
	}
	DirectX::XMFLOAT2 Paddle::Velocity()
	{
		return mVelocity;
	}

}