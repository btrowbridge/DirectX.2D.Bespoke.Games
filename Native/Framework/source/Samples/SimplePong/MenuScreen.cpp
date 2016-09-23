#include "pch.h"
#include "MenuScreen.h"

using namespace std;
using namespace Library;
using namespace DirectX;

namespace Pong {
	const XMFLOAT2 MenuScreen::mMargin(100, 30);


	MenuScreen::MenuScreen(std::shared_ptr<Library::ScreenManager> screenManager) :
		GameScreen(screenManager)
	{
			GameScreen::mTransitionOnTime = 1000ms;
			GameScreen::mTransitionOffTime = 1000ms;
	}

	void MenuScreen::Initialize()
	{
		Game * mGame = GetGame();
		
	    mSpriteBatch = make_unique<SpriteBatch>(mGame->Direct3DDeviceContext());

		mViewport = mGame->Viewport();

		mTitleFont = make_unique<SpriteFont>(mGame->Direct3DDevice(), L"Content\\Fonts\\CenturyGothic_72.spritefont");
		mTextFont = make_unique<SpriteFont>(mGame->Direct3DDevice(), L"Content\\Fonts\\TNR_14.spritefont");

		mKeyboard = reinterpret_cast<KeyboardComponent*>(mGame->Services().GetService(KeyboardComponent::TypeIdClass()));

		GameScreen::Initialize();
	}

	void MenuScreen::OnExit()
	{
		mScreenManager->AddScreen(make_unique<PlayScreen>(mScreenManager), true);
		GameScreen::OnExit();
	}

	void MenuScreen::Draw(const Library::GameTime & gameTime)
	{		
		mSpriteBatch->Begin();
				
		wstring titleText = L"Pong";

		XMFLOAT2 tempViewportSize(mViewport.Width, mViewport.Height);
		XMVECTOR viewportSize= XMLoadFloat2(&tempViewportSize);

		XMVECTOR messageSize = mTitleFont->MeasureString(titleText.c_str());


		XMStoreFloat2(&mTitlePosition, (viewportSize - messageSize) / 2);
		mTitlePosition.y -= XMVectorGetY(messageSize);

		mTitleFont->DrawString(mSpriteBatch.get(), titleText.c_str(), mTitlePosition);

		wstring subText = L"Presss Enter to Play";

		messageSize = mTextFont->MeasureString(subText.c_str());


		XMStoreFloat2(&mTextPosition, (viewportSize - messageSize) / 2);
		mTextPosition.y += XMVectorGetY(messageSize) + mMargin.y;

		mTextFont->DrawString(mSpriteBatch.get(), subText.c_str(), mTextPosition);


		mSpriteBatch->End();

		mScreenManager->FadeScreenToBlack(TransitionPosition());
		GameScreen::Draw(gameTime);
	}

	void MenuScreen::Update(const Library::GameTime & gameTime, bool otherScreenHasFocus, bool coveredByOtherScreen)
	{
		if (mKeyboard->WasKeyPressedThisFrame(Keys::Enter)) {
			ExitScreen();
		}

		GameScreen::Update(gameTime, otherScreenHasFocus, coveredByOtherScreen);
	}

	void MenuScreen::HandleInput(Library::ScreenInputManager & inputManager)
	{
		GameScreen::HandleInput(inputManager);
	}
}