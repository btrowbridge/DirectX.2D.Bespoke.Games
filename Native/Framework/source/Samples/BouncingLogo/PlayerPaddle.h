#pragma once
#include "DrawableGameComponent.h"
#include <d3d11_2.h>
#include "Rectangle.h"
#include <DirectXMath.h>
#include <wrl.h>
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
