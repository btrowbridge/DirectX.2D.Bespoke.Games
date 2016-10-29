#pragma once

#include "DrawableGameComponent.h"
#include <string>
#include <map>
#include <vector>
#include <functional>
#include <Box2D\Box2D.h>
#include "Box2DBehavior.h"

namespace Library
{
	class Box2DComponent;
	class Box2DDebugDraw;
	class KeyboardComponent;
	class MouseComponent;
	class VertexShader;
	class PixelShader;
	class Sprite;
	class Box2DSprite;
	class Texture2D;
}

namespace DirectX
{
	class SpriteFont;
}

namespace AngryBox2DGame
{
	enum class BehaviorType {
		Breakable,
		Ammunition,
		Barrier,
		None
	};

	class Box2DBehavior;
	class Ammunition;

	class AngryBoxDemo final : public Library::DrawableGameComponent
	{
	public:
		AngryBoxDemo(Library::Game& game, const std::shared_ptr<Library::Camera>& camera);

		virtual void Initialize() override;
		virtual void Update(const Library::GameTime& gameTime) override;
		virtual void Draw(const Library::GameTime& gameTime) override;

	private:

		class ContactListener : public b2ContactListener
		{
		public:
			ContactListener() : b2ContactListener() {}

			virtual void BeginContact(b2Contact* contact) override;

			virtual void EndContact(b2Contact* contact) override;

			virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold) override;

			virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) override;

		};

		class QueryCallback final : public b2QueryCallback
		{
		public:
			QueryCallback(const b2Vec2& point);

			virtual bool ReportFixture(b2Fixture* fixture) override;

			b2Vec2 Point;
			b2Fixture* Fixture;
		};

		class DestructionListener final : public b2DestructionListener
		{
		public:
			DestructionListener();

			void SetMouseJoint(b2MouseJoint* mouseJoint);
			void SetMouseJointDestroyedCallback(std::function<void()> callback);

			virtual void SayGoodbye(b2Joint* joint) override;
			virtual void SayGoodbye(b2Fixture* fixture) { UNREFERENCED_PARAMETER(fixture); }

		private:
			b2MouseJoint* mMouseJoint;
			std::function<void()> mMouseJointDestroyedCallback;
		};

		enum class ObjectTypes
		{
			Box,
			Circle,
			Triangle,
			Bolas,
			Stick,
			End
		};

		static const DirectX::XMFLOAT2 TextPosition;
		static const std::wstring HelpText;
		static const std::wstring GameText;
		static const DirectX::XMVECTORF32 BodySpawnPosition;
		static const std::map<ObjectTypes, std::wstring> SpawnObjectNames;

		void AddAmmo();

		void SpawnLevelObjects();

		void AddBarrier();
		void AddGround();
		void AddEdge();
		void AddChain();
		void SpawnObject(ObjectTypes type, DirectX::FXMVECTOR position);
		void SpawnBox(DirectX::FXMVECTOR position);
		void SpawnCircle(DirectX::FXMVECTOR position);
		void SpawnTriangle(DirectX::FXMVECTOR position);
		void SpawnBolas(DirectX::FXMVECTOR position);
		void SpawnStick(DirectX::FXMVECTOR position);
		void ResetWorld();
		void SpawnObjectWithMouse();
		void IncrementMouseSpawnObject();
		void CreateMouseJoint();
		void ApplyForceWithMouse();

		void ToggleDevEnvironment();

		bool mDevEnvironmentActive;

		DirectX::XMVECTOR GetMouseWorldPosition() const;

		Library::Box2DComponent* mPhysicsEngine;
		Library::Box2DDebugDraw* mPhysicsDebugDraw;
		std::unique_ptr<ContactListener> mContactListener;
		Library::KeyboardComponent* mKeyboard;
		Library::MouseComponent* mMouse;
		std::shared_ptr<DirectX::SpriteFont> mHelpFont;		
		std::uint32_t mShapeCount;
		b2Body* mGroundBody;
		ObjectTypes mMouseSpawnObject;
		std::map<ObjectTypes, std::function<void(DirectX::FXMVECTOR position)>> mSpawnMethods;
		std::map<Library::Keys, std::function<void()>> mKeyMappings;
		b2MouseJoint* mMouseJoint;
		DestructionListener mDestructionListener;

		std::vector<std::shared_ptr<Library::Sprite>> mSprites;
		std::vector<std::shared_ptr<Box2DBehavior>> mGameObjects;

		std::shared_ptr<Ammunition> mAmmo;
		b2Vec2 mSlingTarget;
		int mScore;

		std::vector<std::pair<ObjectTypes, DirectX::XMVECTOR>> mLevelObjectsDescription;

		std::shared_ptr<Library::Texture2D> mBoxTexture;
		std::shared_ptr<Library::Texture2D> mCatYellowTexture;
		std::shared_ptr<Library::Texture2D> mDogTexture;
		std::shared_ptr<Library::Texture2D> mStickTexture;
		std::shared_ptr<Library::Texture2D> mTriangleTexture;
		std::shared_ptr<Library::Texture2D> mGroundTexture;
		std::shared_ptr<Library::Texture2D> mFloorTexture;
	};
}