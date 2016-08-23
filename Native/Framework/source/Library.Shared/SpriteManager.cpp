#include "pch.h"
#include "SpriteManager.h"

using namespace std;
using namespace DirectX;

namespace Library
{
	unique_ptr<SpriteManager> SpriteManager::sInstance;

	SpriteManager::SpriteManager(Game& game) :
		mGame(&game), mSpriteBatch(make_shared<DirectX::SpriteBatch>(mGame->Direct3DDeviceContext()))
	{
	}

	shared_ptr<SpriteBatch> SpriteManager::SpriteBatch()
	{
		return sInstance->mSpriteBatch;
	}

	void SpriteManager::Initialize(Game& game)
	{
		sInstance = unique_ptr<SpriteManager>(new SpriteManager(game));		
	}

	void SpriteManager::Shutdown()
	{
		sInstance = nullptr;
	}

	void SpriteManager::DrawTexture2D(ID3D11ShaderResourceView* texture, const XMFLOAT2& position, FXMVECTOR color)
	{
		assert(sInstance != nullptr);

		sInstance->mSpriteBatch->Begin();
		sInstance->mSpriteBatch->Draw(texture, position, color);
		sInstance->mSpriteBatch->End();
	}

	void SpriteManager::DrawTexture2D(ID3D11ShaderResourceView* texture, const Rectangle& destinationRectangle, FXMVECTOR color)
	{
		assert(sInstance != nullptr);

		sInstance->mSpriteBatch->Begin();
		RECT destinationRect = { destinationRectangle.X, destinationRectangle.Y, destinationRectangle.Width, destinationRectangle.Height };
		sInstance->mSpriteBatch->Draw(texture, destinationRect, color);
		sInstance->mSpriteBatch->End();
	}

	void SpriteManager::DrawString(shared_ptr<SpriteFont> font, const wstring& text, const XMFLOAT2& textPosition)
	{
		assert(sInstance != nullptr);

		sInstance->mSpriteBatch->Begin();
		font->DrawString(sInstance->mSpriteBatch.get(), text.c_str(), textPosition);
		sInstance->mSpriteBatch->End();
	}
}