#pragma once

#include <d3d11_2.h>
#include <DirectXMath.h>
#include <DirectXColors.h>
#include <memory>
#include <string>

namespace DirectX
{
	class SpriteBatch;
	class SpriteFont;
}

namespace Library
{
	class Game;
	class Rectangle;

	class SpriteManager
	{
	public:
		static std::shared_ptr<DirectX::SpriteBatch> SpriteBatch();
		static void Initialize(Game& game);
		static void Shutdown();

		static void DrawTexture2D(ID3D11ShaderResourceView* texture, const DirectX::XMFLOAT2& position, DirectX::FXMVECTOR color = DirectX::Colors::White);
		static void DrawTexture2D(ID3D11ShaderResourceView* texture, const Rectangle& destinationRectangle, DirectX::FXMVECTOR color = DirectX::Colors::White);
		static void DrawString(std::shared_ptr<DirectX::SpriteFont> font, const std::wstring& text, const DirectX::XMFLOAT2& textPosition);

		SpriteManager(const SpriteManager&) = delete;
		SpriteManager& operator=(const SpriteManager&) = delete;
		SpriteManager(SpriteManager&&) = delete;
		SpriteManager& operator=(SpriteManager&&) = delete;
		~SpriteManager() = default;

	private:
		SpriteManager(Game& game);

		static std::unique_ptr<SpriteManager> sInstance;

		Game* mGame;
		std::shared_ptr<DirectX::SpriteBatch> mSpriteBatch;
	};
}