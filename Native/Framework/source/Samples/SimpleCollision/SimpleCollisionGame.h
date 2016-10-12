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

		void SetScene();

		void SetListeners();

		void AddBox(float x, float y);
		void AddPlatform(float x, float y);
		void AddWall(float x, float y, b2Vec2 direction);
		void AddFloor(float x, float y);

		void Exit();

	private:
		class ContactListener : public b2ContactListener
		{
		public:
			ContactListener() : b2ContactListener() {}

			virtual void EndContact(b2Contact* contact) override
			{
				b2Body* notSensorBody = (contact->GetFixtureA()->IsSensor()) ? contact->GetFixtureB()->GetBody() :
					((contact->GetFixtureB()->IsSensor()) ? contact->GetFixtureA()->GetBody() : nullptr);

				if (notSensorBody)
					mCallback(notSensorBody);
			}

			virtual void BeginContact(b2Contact* contact) override { B2_NOT_USED(contact); }
			virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold) override
			{
				B2_NOT_USED(contact);
				B2_NOT_USED(oldManifold);
			}
			virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) override
			{
				B2_NOT_USED(contact);
				B2_NOT_USED(impulse);
			}

			void SetCallback(std::function<void(b2Body*)>callback) {
				mCallback = callback;
			}
		private:
			std::function<void(b2Body*)> mCallback;
		};

		ContactListener mOnFloorCollide;

		std::vector<b2Body*> RecyclingBin;
		void EmptyBin();

		static const DirectX::XMVECTORF32 BackgroundColor;

		std::shared_ptr<Library::KeyboardComponent> mKeyboard;
		std::shared_ptr<Library::Box2DComponent> mPhysicsEngine;
		std::shared_ptr<DebugDraw> mDebugDraw;
	};
}