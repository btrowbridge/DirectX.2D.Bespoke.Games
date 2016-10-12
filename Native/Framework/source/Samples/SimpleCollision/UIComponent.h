#pragma once

#include "DrawableGameComponent.h"
#include <DirectXMath.h>
#include <chrono>
#include <memory>

namespace DirectX
{
	class SpriteBatch;
	class SpriteFont;
}

namespace SimpleCollision 
{
	class UIComponent final : public Library::DrawableGameComponent
	{
		RTTI_DECLARATIONS(UIComponent, DrawableGameComponent)

	public:
		UIComponent(Library::Game& game);

		UIComponent() = delete;
		UIComponent(const UIComponent&) = delete;
		UIComponent& operator=(const UIComponent&) = delete;

		DirectX::XMFLOAT2& TextPosition();
		int FrameRate() const;

		virtual void Initialize() override;
		virtual void Update(const Library::GameTime& gameTime) override;
		virtual void Draw(const Library::GameTime& gameTime) override;

	private:
		std::unique_ptr<DirectX::SpriteBatch> mSpriteBatch;
		std::unique_ptr<DirectX::SpriteFont> mSpriteFont;
		DirectX::XMFLOAT2 mTextPosition;

		int mFrameCount;
		int mFrameRate;
		std::chrono::milliseconds mLastTotalGameTime;
	};
}