#pragma once
#include "GameScreen.h"

namespace Pong {

	class Ball;
	class Paddle;
	class ScoreBoard;

	enum GameState {
		Play,
		Paused,
		Player1Win,
		Player2Win
	};
	
	enum PlayerOptions {
		Player1 = 1,
		Player2 = 2,
		AI = 4,
		Player1WithAI = Player1 | AI,
		Player2WithAI = Player2 | AI
	};

	class PlayScreen :
		public Library::GameScreen
	{
	public:


		PlayScreen(std::shared_ptr<Library::ScreenManager> screenManager, PlayerOptions player1Option, PlayerOptions player2Option);

		virtual void Initialize() override;
		virtual void OnExit() override;
		virtual void Draw(const Library::GameTime& gameTime) override;
		virtual void Update(const Library::GameTime& gameTime, bool otherScreenHasFocus, bool coveredByOtherScreen) override;
		virtual void HandleInput(Library::ScreenInputManager& inputManager) override;
		
		void Reset();

		void setGameState(GameState state);
		GameState& getGameState();

		Ball * getBall();
		Paddle * getPlayer1();
		Paddle * getPlayer2();
		ScoreBoard * getScoreBoard();



	private:
		const Library::Point mPlayer1Pos;
		const Library::Point mPlayer2Pos;

		PlayerOptions mPlayer1Options;
		PlayerOptions mPlayer2Options;

		std::shared_ptr<Ball> mBall;
		std::shared_ptr<Paddle> mPlayer1;
		std::shared_ptr<Paddle> mPlayer2;
		std::shared_ptr<ScoreBoard> mScoreBoard;
		
		Library::KeyboardComponent * mKeyboard;

		GameState mGameState;
	};

}