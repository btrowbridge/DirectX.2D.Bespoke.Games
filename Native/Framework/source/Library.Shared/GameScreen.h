#pragma once

#include <chrono>
#include <memory>

namespace Library
{
	class Game;
	class GameTime;
	class ScreenManager;
	class ScreenInputManager;

	enum class ScreenState
	{
		TransitionOn,
		Active,
		TransitionOff,
		Hidden
	};

	class GameScreen
	{
	public:
		explicit GameScreen(std::shared_ptr<Library::ScreenManager> screenManager);
		virtual ~GameScreen() = default;

		bool& IsPopup();
		const std::chrono::milliseconds& TransitionOnTime() const;
		const std::chrono::milliseconds& TransitionOffTime() const;
		float TransitionPosition() const;
		float TransitionAlpha() const;
		ScreenState ScreenState() const;
		bool IsExiting() const;
		bool IsActive() const;
		Game* GetGame() const;
		std::weak_ptr<Library::ScreenManager> ScreenManager();
		const std::vector<std::shared_ptr<GameComponent>>& Components() const;

		virtual void Initialize();
		virtual void OnExit();
		virtual void Draw(const GameTime& gameTime);
		virtual void Update(const GameTime& gameTime, bool otherScreenHasFocus, bool coveredByOtherScreen);
		virtual void HandleInput(ScreenInputManager& inputManager);
		void ExitScreen();

	protected:
		bool mIsPopup;
		Library::ScreenState mScreenState;
		std::chrono::milliseconds mTransitionOnTime;
		std::chrono::milliseconds mTransitionOffTime;
		bool mIsExiting;
		bool mOtherScreenHasFocus;
		float mTransitionPosition;
		std::weak_ptr<Library::ScreenManager> mScreenManager;
		std::vector<std::shared_ptr<GameComponent>> mComponents;

	private:
		bool UpdateTransition(const GameTime& gameTime, const std::chrono::milliseconds& time, int direction);		
	};
}