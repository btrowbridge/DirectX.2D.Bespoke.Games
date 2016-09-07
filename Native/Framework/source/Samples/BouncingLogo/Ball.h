#pragma once
#include "DrawableGameComponent.h"
#include <d3d11_2.h>
#include "Rectangle.h"
#include <DirectXMath.h>
#include <wrl.h>
#include <Audio.h>

namespace BouncingLogo 
{
	class PlayerPaddle;
	class ComputerPaddle;
	class ScoreBoard;

	class Ball final : public Library::DrawableGameComponent
	{
	public:
		
		Ball(Library::Game& game);
		
		virtual void Initialize() override;
		virtual void Update(const Library::GameTime& gameTime) override;
		virtual void Draw(const Library::GameTime& gameTime) override;

		void ResetBall();

		Library::Point Position();
		DirectX::XMFLOAT2 Velocity();


	private:
		
		static const int mMinSpeed;
		static const int mMaxSpeed;

		DirectX::XMFLOAT2 RandomVelocity();

		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mTexture;

		Library::Rectangle mBounds;
		std::unique_ptr<DirectX::SpriteBatch> mSpriteBatch;
		std::unique_ptr<DirectX::SoundEffect> mSoundEffectPing;
		std::unique_ptr<DirectX::SoundEffect> mSoundEffectPong;
		std::unique_ptr<DirectX::SoundEffect> mSoundEffectWall;
		std::unique_ptr<DirectX::SoundEffect> mSoundEffectScore;

		DirectX::XMFLOAT2 mVelocity;

		PlayerPaddle* mPlayer;
		ComputerPaddle* mComputer;
		ScoreBoard* mScoreBoard;

		
	};
}


