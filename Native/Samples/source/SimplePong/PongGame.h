#pragma once

#include "Game.h"
#include "Rectangle.h"

namespace Library
{
	class KeyboardComponent;
	class AudioEngineComponent;
	class ScreenManager;
	class SpriteManager;
}

namespace DirectX
{
	class SpriteBatch;
}

namespace Pong
{
	class MenuScreen;

	class PongGame : public Library::Game
	{
	public:

		PongGame(std::function<void*()> getWindowCallback, std::function<void(SIZE&)> getRenderTargetSizeCallback);

		virtual void Initialize() override;
		virtual void Update(const Library::GameTime& gameTime) override;
		virtual void Draw(const Library::GameTime& gameTime) override;
		virtual void Shutdown() override;

		void Exit();

	private:
		static const DirectX::XMVECTORF32 BackgroundColor;

		std::shared_ptr<Library::KeyboardComponent> mKeyboard;
		std::shared_ptr<Library::AudioEngineComponent> mAudio;

		std::shared_ptr<Pong::MenuScreen> mMenuScreen;
		std::shared_ptr<Library::SpriteManager> mSpriteManager;

		std::weak_ptr<Library::ScreenManager> mScreenManager;

	};
}