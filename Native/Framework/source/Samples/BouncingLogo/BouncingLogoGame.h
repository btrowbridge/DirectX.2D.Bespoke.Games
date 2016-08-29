#pragma once

#include "Game.h"

namespace Library
{
	class KeyboardComponent;
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

		void Exit();

	private:
		static const DirectX::XMVECTORF32 BackgroundColor;

		std::shared_ptr<Library::KeyboardComponent> mKeyboard;

	};
}