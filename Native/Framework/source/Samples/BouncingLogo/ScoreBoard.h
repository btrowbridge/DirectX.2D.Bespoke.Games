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

namespace BouncingLogo {
	class ScoreBoard :
		public Library::DrawableGameComponent
	{
	public:
		ScoreBoard(Library::Game& game);

		virtual void Initialize() override;
		virtual void Update(const Library::GameTime& gameTime) override;
		virtual void Draw(const Library::GameTime& gameTime) override;

		
		void PlayerScores();
		void ComputerScores();

		void ResetScores();
		
	private:
		std::unique_ptr<DirectX::SpriteBatch> mSpriteBatch;
		std::unique_ptr<DirectX::SpriteFont> mSpriteFont;
		DirectX::XMFLOAT2 mPlayerScorePosition;
		DirectX::XMFLOAT2 mComputerScorePosition;

		static const DirectX::XMFLOAT2 mMargin;

		static const int mScoreToWin;

		int mPlayerScore;
		int mComputerScore;
	};
}

