#include "pch.h"
#include "GameScreen.h"

using namespace std;
using namespace std::chrono;

namespace Library
{
	GameScreen::GameScreen(shared_ptr<Library::ScreenManager> screenManager) :
		mScreenManager(screenManager), mIsPopup(false), mScreenState(ScreenState::TransitionOn),
		mTransitionOnTime(0), mTransitionOffTime(0), mIsExiting(false),
		mOtherScreenHasFocus(false), mTransitionPosition(1.0f)
	{
	}

	bool& GameScreen::IsPopup()
	{
		return mIsPopup;
	}

	const milliseconds& GameScreen::TransitionOnTime() const
	{
		return mTransitionOnTime;
	}

	const milliseconds& GameScreen::TransitionOffTime() const
	{
		return mTransitionOffTime;
	}

	float GameScreen::TransitionPosition() const
	{
		return mTransitionPosition;
	}

	float GameScreen::TransitionAlpha() const
	{
		return 1.0f - mTransitionPosition;
	}

	ScreenState GameScreen::ScreenState() const
	{
		return mScreenState;
	}

	bool GameScreen::IsExiting() const
	{
		return mIsExiting;
	}

	bool GameScreen::IsActive() const
	{
		return !mOtherScreenHasFocus && (mScreenState == ScreenState::TransitionOn || mScreenState == ScreenState::Active);
	}

	Game* GameScreen::GetGame() const
	{
		return (mScreenManager != nullptr ? mScreenManager->GetGame() : nullptr);
	}

	shared_ptr<Library::ScreenManager> GameScreen::ScreenManager()
	{
		return mScreenManager;
	}

	const std::vector<std::shared_ptr<GameComponent>>& GameScreen::Components() const
	{
		return mComponents;
	}

	void GameScreen::Initialize()
	{
		for (auto& component : mComponents)
		{
			component->Initialize();
		}
	}

	void GameScreen::OnExit()
	{
	}

	void GameScreen::Update(const GameTime& gameTime, bool otherScreenHasFocus, bool coveredByOtherScreen)
	{
		for (auto& component : mComponents)
		{
			if (component->Enabled())
			{
				component->Update(gameTime);
			}
		}

		mOtherScreenHasFocus = otherScreenHasFocus;

		if (mIsExiting)
		{
			mScreenState = ScreenState::TransitionOff;

			// If the screen is going away to die, it should transition off
			if (UpdateTransition(gameTime, mTransitionOffTime, 1) == false)
			{
				OnExit();
				mIsExiting = false;
				if (mScreenManager != nullptr)
				{
					mScreenManager->RemoveScreen(*this);
				}
			}
		}
		else if (coveredByOtherScreen)
		{
			// If the screen is covered by another, it should transition off
			if (UpdateTransition(gameTime, mTransitionOffTime, 1))
			{
				// Still busy transitioning
				mScreenState = ScreenState::TransitionOff;
			}
			else
			{
				// Transition finished
				mScreenState = ScreenState::Hidden;
			}
		}
		else
		{
			// Otherwise the screen should transition on and become active
			if (UpdateTransition(gameTime, mTransitionOnTime, -1))
			{
				// Still busy transitioning
				mScreenState = ScreenState::TransitionOn;
			}
			else
			{
				// Transition finished
				mScreenState = ScreenState::Active;
			}
		}
	}

	void GameScreen::Draw(const GameTime& gameTime)
	{
		for (auto& component : mComponents)
		{
			DrawableGameComponent* drawableGameComponent = component->As<DrawableGameComponent>();
			if (drawableGameComponent != nullptr && drawableGameComponent->Visible())
			{
				drawableGameComponent->Draw(gameTime);
			}
		}
	}

	void GameScreen::HandleInput(ScreenInputManager& inputManager)
	{
		UNREFERENCED_PARAMETER(inputManager);
	}

	void GameScreen::ExitScreen()
	{
		if (mTransitionOffTime == milliseconds::zero())
		{
			if (mScreenManager != nullptr)
			{
				mScreenManager->RemoveScreen(*this);
			}			
		}
		else
		{
			mIsExiting = true;
		}
	}

	bool GameScreen::UpdateTransition(const GameTime& gameTime, const milliseconds& time, int direction)
	{
		float transitionDelta;

		// How much should we move by?
		if (time == milliseconds::zero())
		{
			transitionDelta = 1;
		}
		else
		{
			transitionDelta = static_cast<float>(gameTime.ElapsedGameTime().count()) / time.count();
		}

		// Update the transition position.
		mTransitionPosition += transitionDelta * direction;

		// Did we reach the end of the transition?
		if ((mTransitionPosition <= 0) || (mTransitionPosition >= 1))
		{
			mTransitionPosition = clamp(mTransitionPosition, 0.0f, 1.0f);
			return false;
		}

		// Otherwise we are still busy transitioning.
		return true;
	}
}