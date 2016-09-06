#include "pch.h"
#include "Ball.h"
#include "SpriteBatch.h"
#include "BouncingLogoGame.h"

using namespace Library;
using namespace std;
using namespace Microsoft::WRL;
using namespace DirectX;

BouncingLogo::Ball::Ball(Library::Game & game) : 
	DrawableGameComponent(game), mBounds(Rectangle::Empty)
{
}

void BouncingLogo::Ball::Initialize()
{
	// Load a texture
	ComPtr<ID3D11Resource> textureResource;
	wstring textureName = L"Content\\Textures\\Ball.png";

	ThrowIfFailed(CreateWICTextureFromFile(mGame->Direct3DDevice(), textureName.c_str(),
		textureResource.ReleaseAndGetAddressOf(), mTexture.ReleaseAndGetAddressOf()),
		"CreateWICTextureFromFile() Failed.");

	ComPtr<ID3D11Texture2D> texture;
	ThrowIfFailed(textureResource.As(&texture), "Invalid ID3D11 texture resource");

	mBounds = TextureHelper::GetTextureBounds(texture.Get());

	Point mTextureHalfSize;
	mTextureHalfSize.X = mBounds.Width / 2;
	mTextureHalfSize.Y = mBounds.Height / 2;

	auto& mViewport = mGame->Viewport();

	Library::Rectangle viewportSize(static_cast<int>(mViewport.TopLeftX), static_cast<int>(mViewport.TopLeftY),
		static_cast<int>(mViewport.Width), static_cast<int>(mViewport.Height));
	Point center = viewportSize.Center();

	mBounds.X = center.X - mTextureHalfSize.X;
	mBounds.Y = center.Y - mTextureHalfSize.Y;

	mVelocity.x = 120;
	mVelocity.y = 200;

	mSpriteBatch = make_unique<SpriteBatch>(mGame->Direct3DDeviceContext());
}

void BouncingLogo::Ball::Update(const Library::GameTime & gameTime)
{
	float elapsedTime = gameTime.ElapsedGameTimeSeconds().count();

	auto& mViewport = mGame->Viewport();
	mPlayer = mGame->As<BouncingLogoGame>()->getPlayer();
	mComputer = mGame->As<BouncingLogoGame>()->getComputer();
	float mSpeed = getSpeed();

	XMFLOAT2 positionDelta(mVelocity.x * elapsedTime, mVelocity.y * elapsedTime);
	mBounds.X += static_cast<int>(std::round(positionDelta.x));
	mBounds.Y += static_cast<int>(std::round(positionDelta.y));

	

	if (mBounds.X + mBounds.Width > mViewport.Width && mVelocity.x > 0.0f)
	{
		mVelocity.x *= -1;
	}
	else if (mBounds.X < 0 && mVelocity.x < 0.0f)
	{
		mVelocity.x *= -1;
	}

	if (mBounds.Y + mBounds.Height > mViewport.Height && mVelocity.y > 0.0f)
	{
		mVelocity.y *= -1;
	}
	else if (mBounds.Y < 0 && mVelocity.y < 0.0f)
	{
		mVelocity.y *= -1;
	}
	
	if (mBounds.Intersects(mPlayer->Bounds()))
	{
		
		if ((abs(mBounds.Left() - mPlayer->Bounds().Right()) >	//Bounce off the bottom or top
			abs(mBounds.Bottom() - mPlayer->Bounds().Top())) || 
			(abs(mBounds.Left() - mPlayer->Bounds().Right())>
			abs(mBounds.Top() - mPlayer->Bounds().Bottom()))) 
		{
			mVelocity.y = abs(mVelocity.y) * (((mBounds.Center().Y - mPlayer->Bounds().Center().Y < 0)) ? -1 : 1);
		}
		else {
			mVelocity.x *= -1;
		}

		if (mPlayer->Velocity().y != 0 )
		{
			mVelocity.y = (mPlayer->Velocity().y + mVelocity.y) / 2; //transfer some paddle momentum
		}
	}
	else if (mBounds.Intersects(mComputer->Bounds()))
	{
		if ((abs(mBounds.Right() - mComputer->Bounds().Left()) >	//Bounce off the bottom or top
			abs(mBounds.Bottom() - mComputer->Bounds().Top()))||
			(abs(mBounds.Right() - mComputer->Bounds().Left()) >
				abs(mBounds.Top() - mComputer->Bounds().Bottom()))) 
		{
			mVelocity.y = abs(mVelocity.y) * (((mBounds.Center().Y - mComputer->Bounds().Center().Y < 0)) ? -1 : 1);
		}
		else {
			mVelocity.x *= -1;
			
		}
		if (mComputer->Velocity().y != 0)
		{
			mVelocity.y = (mComputer->Velocity().y + mVelocity.y)/2;//transfer some paddle momentum
		}
		

	}
	

}

void BouncingLogo::Ball::Draw(const Library::GameTime & gameTime)
{
	XMFLOAT2 position(mBounds.X, mBounds.Y);

	mSpriteBatch->Begin();
	mSpriteBatch->Draw(mTexture.Get(), position);
	mSpriteBatch->End();

	//SpriteManager::DrawTexture2D(mTexture.Get(), position);

}

Library::Point BouncingLogo::Ball::Position()
{
	return mBounds.Center();
}

DirectX::XMFLOAT2 BouncingLogo::Ball::Velocity()
{
	return mVelocity;
}

float BouncingLogo::Ball::getSpeed()
{
	return sqrt(pow(mVelocity.x,2) + pow(mVelocity.y,2));
}
