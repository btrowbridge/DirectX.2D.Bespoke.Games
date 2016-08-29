#pragma once

#include "Game.h"
#include "Rectangle.h"

namespace Library
{
	class KeyboardComponent;
}

namespace DirectX 
{
	class SpriteBatch;
}

namespace BouncingLogo
{
	class BouncingLogoGame : public Library::Game
	{
	public:
		BouncingLogoGame(std::function<void*()> getWindowCallback, std::function<void(SIZE&)> getRenderTargetSizeCallback);

		virtual void Initialize() override;
		virtual void Update(const Library::GameTime& gameTime) override;
		virtual void Draw(const Library::GameTime& gameTime) override;
		virtual void Shutdown() override;

		void Exit();

	private:
		static const DirectX::XMVECTORF32 BackgroundColor;

		std::shared_ptr<Library::KeyboardComponent> mKeyboard;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mTexture;
		Library::Rectangle mBounds;
		//std::unique_ptr<SpriteBatch> mSpriteBatch;
		DirectX::XMFLOAT2 mVelocity;

	};
}