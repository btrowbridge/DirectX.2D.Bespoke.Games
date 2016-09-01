#pragma once
#include "DrawableGameComponent.h"
#include <d3d11_2.h>
#include "Rectangle.h"
#include <DirectXMath.h>
#include <wrl.h>


namespace BouncingLogo
{


	class Paddle : public Library::DrawableGameComponent
	{
	public:
		Paddle(Library::Game& game);

		virtual void Initialize() override;
		virtual void Update(const Library::GameTime& gameTime) override;
		virtual void Draw(const Library::GameTime& gameTime) override;
		int RightSide();
		int LeftSide();

	private:
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mTexture;
		std::unique_ptr<DirectX::SpriteBatch> mSpriteBatch;

	protected:
		Library::Rectangle mBounds;
		DirectX::XMFLOAT2 mVelocity;
	};
}


