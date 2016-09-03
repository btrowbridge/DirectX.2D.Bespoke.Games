#include "pch.h"
#include "Paddle.h"
#include "SpriteBatch.h"

using namespace Library;
using namespace std;
using namespace Microsoft::WRL;
using namespace DirectX;

namespace BouncingLogo {
	BouncingLogo::Paddle::Paddle(Library::Game & game) : DrawableGameComponent(game), mBounds(Rectangle::Empty)
	{
	}

	void BouncingLogo::Paddle::Initialize()
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

	void BouncingLogo::Paddle::Update(const Library::GameTime & gameTime)
	{
	}

	void BouncingLogo::Paddle::Draw(const Library::GameTime & gameTime)
	{
		XMFLOAT2 position(mBounds.X, mBounds.Y);

		mSpriteBatch->Begin();
		mSpriteBatch->Draw(mTexture.Get(), position);
		mSpriteBatch->End();
	}

	Library::Rectangle* BouncingLogo::Paddle::Bounds()
	{
		return &mBounds;
	}
}