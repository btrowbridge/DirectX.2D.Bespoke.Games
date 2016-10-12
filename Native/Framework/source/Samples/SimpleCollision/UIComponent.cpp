#include "pch.h"
#include "UIComponent.h"

using namespace std;
using namespace DirectX;
using namespace Library;

namespace SimpleCollision
{
	RTTI_DEFINITIONS(UIComponent)

	UIComponent::UIComponent(Game& game) :
		DrawableGameComponent(game),
		mTextPosition(0.0f, 20.0f), mFrameCount(0), mFrameRate(0)
	{
	}

	XMFLOAT2& UIComponent::TextPosition()
	{
		return mTextPosition;
	}

	int UIComponent::FrameRate() const
	{
		return mFrameCount;
	}

	void UIComponent::Initialize()
	{
		mSpriteBatch = make_unique<SpriteBatch>(mGame->Direct3DDeviceContext());
		mSpriteFont = make_unique<SpriteFont>(mGame->Direct3DDevice(), L"Content\\Fonts\\Arial_14_Regular.spritefont");
	}

	void UIComponent::Update(const GameTime& gameTime)
	{
		if ((gameTime.TotalGameTime() - mLastTotalGameTime).count() >= 1000)
		{
			mLastTotalGameTime = gameTime.TotalGameTime();
			mFrameRate = mFrameCount;
			mFrameCount = 0;
		}

		++mFrameCount;
	}

	void UIComponent::Draw(const GameTime& gameTime)
	{
		mSpriteBatch->Begin();

		wostringstream fpsLabel;
		fpsLabel << setprecision(4) << L"Frame Rate: " << mFrameRate << "    Total Elapsed Time: " << gameTime.TotalGameTimeSeconds().count() << endl;
		fpsLabel << L"Press Space to add a Box";
		mSpriteFont->DrawString(mSpriteBatch.get(), fpsLabel.str().c_str(), mTextPosition);

		mSpriteBatch->End();
	}
}
