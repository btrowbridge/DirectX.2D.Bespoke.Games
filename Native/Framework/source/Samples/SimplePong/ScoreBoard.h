#pragma once
#include "DrawableGameComponent.h"
#include <d3d11_2.h>
#include <DirectXMath.h>
#include <wrl.h>

namespace DirectX
{
	class SpriteBatch;
	class SpriteFont;
}

namespace Pong {
	class ScoreBoard :
		public Library::DrawableGameComponent
	{
	public:
		ScoreBoard(Library::Game& game);

		virtual void Initialize() override;
		virtual void Update(const Library::GameTime& gameTime) override;
		virtual void Draw(const Library::GameTime& gameTime) override;

		
		void Player1Scores();
		void Player2Scores();

		void ResetScores();
		
	private:
		std::unique_ptr<DirectX::SpriteBatch> mSpriteBatch;
		std::unique_ptr<DirectX::SpriteFont> mSpriteFont;
		DirectX::XMFLOAT2 mPlayer1ScorePosition;
		DirectX::XMFLOAT2 mPlayer2ScorePosition;
		DirectX::XMFLOAT2 mCenterTextPosition;


		static const DirectX::XMFLOAT2 mMargin;

		static const int mScoreToWin;

		int mPlayer1Score;
		int mPlayer2Score;
	};
}

