#pragma once
#include "GameScreen.h"


namespace DirectX
{
	class SpriteBatch;
	class SpriteFont;
}

namespace Pong {

	class MenuScreen :
		public Library::GameScreen
	{
	public:

		MenuScreen(std::shared_ptr<Library::ScreenManager> screenManager);

		virtual void Initialize() override;
		virtual void OnExit() override;
		virtual void Draw(const Library::GameTime& gameTime) override;
		virtual void Update(const Library::GameTime& gameTime, bool otherScreenHasFocus, bool coveredByOtherScreen) override;
		virtual void HandleInput(Library::ScreenInputManager& inputManager) override;

	private:
		std::unique_ptr<DirectX::SpriteBatch> mSpriteBatch;
		std::unique_ptr<DirectX::SpriteFont> mTitleFont;
		std::unique_ptr<DirectX::SpriteFont> mTextFont;


		DirectX::XMFLOAT2 mTitlePosition;
		DirectX::XMFLOAT2 mTextPosition;

		D3D11_VIEWPORT mViewport;

		static const DirectX::XMFLOAT2 mMargin;
		
		PlayerOptions mPlayer1Choice;
		PlayerOptions mPlayer2Choice;

		Library::KeyboardComponent * mKeyboard;
	};

}