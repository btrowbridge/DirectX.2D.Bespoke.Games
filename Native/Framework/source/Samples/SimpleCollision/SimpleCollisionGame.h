#pragma once

#include "Game.h"
#include "Rectangle.h"

namespace Library
{
	class KeyboardComponent;
	class Box2DComponent;
}


namespace SimpleCollision
{
	class DebugDraw;

	class SimpleCollisionGame : public Library::Game 
	{
	public:

		SimpleCollisionGame(std::function<void*()> getWindowCallback, std::function<void(SIZE&)> getRenderTargetSizeCallback);

		virtual void Initialize() override;
		virtual void Update(const Library::GameTime& gameTime) override;
		virtual void Draw(const Library::GameTime& gameTime) override;
		virtual void Shutdown() override;

		void AddBox(float x, float y);
		void AddPlatform(float x, float y);
		void AddWall(float x, float y);

		void Exit();

	private:
		static const DirectX::XMVECTORF32 BackgroundColor;

		std::shared_ptr<Library::KeyboardComponent> mKeyboard;
		std::shared_ptr<Library::Box2DComponent> mPhysicsEngine;
		std::shared_ptr<DebugDraw> mDebugDraw;
	};
}