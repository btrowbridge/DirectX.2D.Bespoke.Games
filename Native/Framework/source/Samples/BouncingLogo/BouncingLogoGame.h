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
	class Ball;
	class PlayerPaddle;
	class ComputerPaddle;

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
		std::shared_ptr<Ball> mBall;
		std::shared_ptr<PlayerPaddle> mPlayer;
		std::shared_ptr<ComputerPaddle> mComPaddle;

	};
}