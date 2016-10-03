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
	class PlayScreen;

	class ScoreBoard :
		public Library::DrawableGameComponent
	{
	public:
		ScoreBoard(Library::Game& game, PlayScreen* screen);

		virtual void Initialize() override;
		virtual void Update(const Library::GameTime& gameTime) override;
		virtual void Draw(const Library::GameTime& gameTime) override;

		void Player1Scores();
		void Player2Scores();

		void ResetScores();

	private:

		enum ScorePosition
		{
			ScorePositionLeft = 1,
			ScorePositionRight = 2,
			ScorePositionBoth = ScorePositionLeft | ScorePositionRight
		};

		void UpdateScorePositions(ScorePosition positionToUpdate);

		std::unique_ptr<DirectX::SpriteBatch> mSpriteBatch;
		std::unique_ptr<DirectX::SpriteFont> mSpriteFont;
		DirectX::XMFLOAT2 mPlayer1ScorePosition;
		DirectX::XMFLOAT2 mPlayer2ScorePosition;
		DirectX::XMFLOAT2 mCenterTextPosition;

		D3D11_VIEWPORT mViewport;

		static const DirectX::XMFLOAT2 mMargin;

		const int mScoreToWin;

		int mPlayer1Score;
		int mPlayer2Score;

		PlayScreen * mScreen;
	};
}
