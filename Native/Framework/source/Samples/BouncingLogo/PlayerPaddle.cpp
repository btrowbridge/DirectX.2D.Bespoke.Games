#include "pch.h"
#include "PlayerPaddle.h"
#include "SpriteBatch.h"

using namespace Library;
using namespace std;
using namespace Microsoft::WRL;
using namespace DirectX;

BouncingLogo::PlayerPaddle::PlayerPaddle(Library::Game & game) : Paddle(game)
{
}

void BouncingLogo::PlayerPaddle::Initialize()
{
	// Load a texture
	ComPtr<ID3D11Resource> textureResource;
	wstring textureName = L"Content\\Textures\\PlayerPaddle.png";

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

	mVelocity.x = 0;
	mVelocity.y = 0;

	mSpriteBatch = make_unique<SpriteBatch>(mGame->Direct3DDeviceContext());
}

void BouncingLogo::PlayerPaddle::Update(const Library::GameTime & gameTime)
{
}

void BouncingLogo::PlayerPaddle::Draw(const Library::GameTime & gameTime)
{
	XMFLOAT2 position(mBounds.X, mBounds.Y);

	mSpriteBatch->Begin();
	mSpriteBatch->Draw(mTexture.Get(), position);
	mSpriteBatch->End();

}
