#pragma once
#include "DrawableGameComponent.h"
#include <d3d11_2.h>
#include "Rectangle.h"
#include <DirectXMath.h>
#include <wrl.h>


namespace BouncingLogo
{


	class PlayerPaddle final : public Library::DrawableGameComponent
	{
	public:
		PlayerPaddle(Library::Game& game);


		virtual void Initialize() override;
		virtual void Update(const Library::GameTime& gameTime) override;
		
	};
}
