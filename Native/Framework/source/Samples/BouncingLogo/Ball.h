#pragma once
#include "DrawableGameComponent.h"
#include <d3d11_2.h>
#include "Rectangle.h"
#include <DirectXMath.h>
#include <wrl.h>

namespace BouncingLogo 
{
	class PlayerPaddle;
	class ComputerPaddle;

	class Ball final : public Library::DrawableGameComponent
	{
	public:
		
		Ball(Library::Game& game);
		
		virtual void Initialize() override;
		virtual void Update(const Library::GameTime& gameTime) override;
		virtual void Draw(const Library::GameTime& gameTime) override;

		Library::Point Position();
		DirectX::XMFLOAT2 Velocity();

	private:
		float getSpeed();

		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mTexture;

		Library::Rectangle mBounds;
		std::unique_ptr<DirectX::SpriteBatch> mSpriteBatch;
		DirectX::XMFLOAT2 mVelocity;

		PlayerPaddle* mPlayer;
		ComputerPaddle* mComputer;
	};
}


