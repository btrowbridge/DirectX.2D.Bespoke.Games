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
		PlayerPaddle( Library::Game& game, std::shared_ptr<Library::KeyboardComponent> Keyboard);


		virtual void Initialize() override;
		virtual void Update(const Library::GameTime& gameTime) override;

	private:
		static const int mSpeed;
		std::shared_ptr<Library::KeyboardComponent> mKeyboard;
	};
}
