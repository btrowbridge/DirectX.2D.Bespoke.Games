#include "pch.h"
#include "ScreenManager.h"

using namespace std;
using namespace DirectX;

namespace Library
{
	RTTI_DEFINITIONS(ScreenManager)

		ScreenManager::ScreenManager(Game& game) :
		ScreenManager(game, nullptr)
	{
	}

	ScreenManager::ScreenManager(Game& game, const std::shared_ptr<Camera>& camera) :
		DrawableGameComponent(game, camera)
	{
	}

	XMFLOAT2 ScreenManager::ScreenCenter() const
	{
		const auto& viewport = mGame->Viewport();
		return XMFLOAT2(viewport.TopLeftX + (viewport.Width / 2), viewport.TopLeftY + (viewport.Height / 2));
	}

	const std::vector<std::shared_ptr<GameScreen>>& ScreenManager::Screens() const
	{
		return mScreens;
	}

	const IServiceProvider& ScreenManager::Services() const
	{
		return mGame->Services();
	}

	void ScreenManager::Initialize()
	{
		mScreenInputManager = make_shared<ScreenInputManager>(/*TODO*/);

		for (auto& screen : mScreens)
		{
			screen->Initialize();
		}

		wstring textureName = L"Content\\Textures\\Blank.png";
		ThrowIfFailed(CreateWICTextureFromFile(mGame->Direct3DDevice(), textureName.c_str(), nullptr, mBlankTexture.ReleaseAndGetAddressOf()), "CreateDDSTextureFromFile() failed.");
	}

	void ScreenManager::Update(const GameTime& gameTime)
	{
		mScreensToUpdate.clear();
		copy(mScreens.cbegin(), mScreens.cend(), back_inserter(mScreensToUpdate));

		bool otherScreenHasFocus = !mGame->IsActive();
		bool coveredByOtherScreen = false;

		while (mScreensToUpdate.empty() == false)
		{
			// Pop the topmost screen off the update list and update it
			auto& screen = mScreensToUpdate.back();
			mScreensToUpdate.pop_back();
			screen->Update(gameTime, otherScreenHasFocus, coveredByOtherScreen);

			if (screen->ScreenState() == ScreenState::TransitionOn || screen->ScreenState() == ScreenState::Active)
			{
				// If this is the first active screen we've encountered,
				// give it a chance to handle input
				if (otherScreenHasFocus == false)
				{
					screen->HandleInput(*mScreenInputManager);
					otherScreenHasFocus = true;
				}

				// If this is an active non-popup, inform any subsequent
				// screens that they are covered by it
				if (screen->IsPopup() == false)
				{
					coveredByOtherScreen = true;
				}
			}
		}
	}

	void ScreenManager::Draw(const GameTime& gameTime)
	{
		for (auto& screen : mScreens)
		{
			if (screen->ScreenState() != ScreenState::Hidden)
			{
				screen->Draw(gameTime);
			}
		}
	}

	void ScreenManager::AddScreen(shared_ptr<GameScreen> screen, bool shouldInitializeScreen)
	{
		if (shouldInitializeScreen)
		{
			screen->Initialize();
		}

		mScreens.push_back(screen);
	}

	void ScreenManager::AddScreens(const vector<shared_ptr<GameScreen>>& screens, bool shouldInitializeScreens)
	{
		for (auto& screen : screens)
		{
			if (ContainsScreen(screen) == false)
			{
				AddScreen(screen, shouldInitializeScreens);
			}
		}
	}

	void ScreenManager::RemoveScreen(GameScreen& screen)
	{
		auto predicate = [&screen](const shared_ptr<GameScreen>& s) { return s.get() == &screen; };
		auto found = find_if(mScreens.cbegin(), mScreens.cend(), predicate);
		if (found != mScreens.cend())
		{
			mScreens.erase(found);
		}

		found = find_if(mScreensToUpdate.cbegin(), mScreensToUpdate.cend(), predicate);
		if (found != mScreensToUpdate.cend())
		{
			mScreensToUpdate.erase(found);
		}
	}

	void ScreenManager::RemoveAllScreens()
	{
		mScreensToUpdate.clear();
		mScreens.clear();
	}

	bool ScreenManager::ContainsScreen(shared_ptr<GameScreen> screen) const
	{
		return find(begin(mScreens), end(mScreens), screen) != mScreens.end();
	}

	void ScreenManager::FadeScreenToBlack(float alpha)
	{
		XMVECTORF32 color = { 0, 0, 0, alpha };
		SpriteManager::DrawTexture2D(mBlankTexture.Get(), Rectangle(0, 0, static_cast<int>(mGame->Viewport().Width), static_cast<int>(mGame->Viewport().Height)), color);
	}
}