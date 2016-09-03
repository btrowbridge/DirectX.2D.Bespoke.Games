#pragma once
#include "Paddle.h"


namespace BouncingLogo
{

	class PlayerPaddle final : public Paddle
	{
	public:
		PlayerPaddle( Library::Game& game, Library::KeyboardComponent & keyboard);


		virtual void Initialize() override;
		virtual void Update(const Library::GameTime& gameTime) override;

	private:
		static const int mSpeed;
		Library::KeyboardComponent * mKeyboard;
	};
}
