#pragma once
#include "DrawableGameComponent.h"
#include <d3d11_2.h>
#include "Rectangle.h"
#include <DirectXMath.h>
#include <wrl.h>
#include "Paddle.h"


namespace BouncingLogo
{


	class ComputerPaddle final : public Paddle
	{
	public:
		ComputerPaddle(Library::Game& game, Ball & ball);


		virtual void Initialize() override;
		virtual void Update(const Library::GameTime& gameTime) override;

	private:
		static const int mSpeed;
		Ball * mBall;
	};
}
