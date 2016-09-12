#pragma once
#include "DrawableGameComponent.h"
#include <d3d11_2.h>
#include "Rectangle.h"
#include <DirectXMath.h>
#include <wrl.h>

namespace Pong
{

	class Paddle final : public Library::DrawableGameComponent
	{
	public:
		Paddle(Library::Game& game, PlayerOptions playerOption);

		virtual void Initialize() override;
		virtual void Update(const Library::GameTime& gameTime) override;
		virtual void Draw(const Library::GameTime& gameTime) override;

		virtual void ResetPaddle();

		Library::Rectangle Bounds();
		DirectX::XMFLOAT2 Velocity();

	private:
		Library::Keys mUpKey;
	    Library::Keys mDownKey;
		PlayerOptions mPlayerOption;

		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mTexture;
		std::unique_ptr<DirectX::SpriteBatch> mSpriteBatch;
		
		Library::Point mTextureHalfSize;
		Library::Point mDefaultPosition;
		Library::Rectangle mBounds;
		

		static const int mSpeed;
		Library::KeyboardComponent * mKeyboard;

		DirectX::XMFLOAT2 mVelocity;

		bool isAIEnabled;
		Ball * mBall;
	};
}
