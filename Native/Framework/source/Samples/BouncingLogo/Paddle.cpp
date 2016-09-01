#include "pch.h"
#include "Paddle.h"
#include "SpriteBatch.h"

using namespace Library;
using namespace std;
using namespace Microsoft::WRL;
using namespace DirectX;

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

int BouncingLogo::Paddle::RightSide()
{
	return 0;
}

int BouncingLogo::Paddle::LeftSide()
{
	return 0;
}
