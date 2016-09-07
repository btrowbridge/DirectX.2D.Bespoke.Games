#pragma once
#include "Paddle.h"

namespace BouncingLogo
{
	class Ball;

	class ComputerPaddle final : public Paddle
	{
	public:
		ComputerPaddle(Library::Game& game);

		virtual void Initialize() override;
		virtual void Update(const Library::GameTime& gameTime) override;

	private:

		static const int mSpeed;
		Ball * mBall;
	};
}
