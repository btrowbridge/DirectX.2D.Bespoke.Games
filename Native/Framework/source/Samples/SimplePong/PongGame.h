#pragma once

#include "Game.h"
#include "Rectangle.h"

namespace Library
{
	class KeyboardComponent;
	class AudioEngineComponent;
}

namespace DirectX
{
	class SpriteBatch;
}

namespace Pong
{
	class Ball;
	class Paddle;
	class ScoreBoard;

	const enum GameState {
		Play,
		Paused,
		Player1Win,
		Player2Win
	};
	
	const enum PlayerOptions {
		Player1 = 1,
		Player2 = 2,
		AI = 4,
		Player1WithAI = Player1 | AI,
		Player2WithAI = Player2 | AI
	};

	class PongGame : public Library::Game
	{
	public:

		PongGame(std::function<void*()> getWindowCallback, std::function<void(SIZE&)> getRenderTargetSizeCallback);

		virtual void Initialize() override;
		virtual void Update(const Library::GameTime& gameTime) override;
		virtual void Draw(const Library::GameTime& gameTime) override;
		virtual void Shutdown() override;

		void Exit();

		void Reset();

		void setGameState(GameState state);
		GameState& GameState();

		Ball* getBall();
		Paddle* getPlayer1();
		Paddle* getPlayer2();
		ScoreBoard* getScoreBoard();

	private:
		static const DirectX::XMVECTORF32 BackgroundColor;

		const Library::Point mPlayer1Pos;
		const Library::Point mPlayer2Pos;

		std::shared_ptr<Library::KeyboardComponent> mKeyboard;
		std::shared_ptr<Library::AudioEngineComponent>mAudio;

		std::shared_ptr<Ball> mBall;
		std::shared_ptr<Paddle> mPlayer1;
		std::shared_ptr<Paddle> mPlayer2;
		std::shared_ptr<ScoreBoard> mScoreBoard;

		Pong::GameState mGameState;
	};
}