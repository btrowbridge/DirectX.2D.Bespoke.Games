#include "pch.h"
#include "ReflectionDemo.h"

using namespace std;
using namespace Library;
using namespace DirectX;

namespace Reflection2DGame
{
	const XMFLOAT2 ReflectionDemo::TextPosition = { 0.0f, 42.0f };
	const std::wstring ReflectionDemo::HelpText = L"Toggle Dev Environment (Tab) \nAdd Box (Space)\nAdd Circle (Enter)\nAdd Triangle (Backspace)\nAdd Stick (K)\nAdd Bolas (Insert)\nToggle Debug Draw (V)\nToggle AABBs (B)\nToggle Center of Mass (C)\nToggle Joints (J)\nSpawn w/ Mouse (Left Mouse Button)\nChange Mouse Spawn Object (+)\nGrab Object (Right Mouse Button)";
	const std::wstring ReflectionDemo::GameText = L"Toggle Dev Environment (Tab) \nGrab Ammo Bird (Right Mouse Button)";
	const XMVECTORF32 ReflectionDemo::BodySpawnPosition = { 0.0f, 8.0f, 0.0f, 1.0f };
	const map<ReflectionDemo::ObjectTypes, std::wstring> ReflectionDemo::SpawnObjectNames =
	{
		{ ObjectTypes::Box, L"Box" },
		{ ObjectTypes::Circle, L"Circle" },
		{ ObjectTypes::Triangle, L"Triangle" },
		{ ObjectTypes::Stick, L"Stick" },
		{ ObjectTypes::End, L"" },
	};


	ReflectionDemo::ReflectionDemo(Game& game, const shared_ptr<Camera>& camera) :
		DrawableGameComponent(game, camera),
		mPhysicsEngine(nullptr), mPhysicsDebugDraw(nullptr), mKeyboard(nullptr),
		mShapeCount(0U), mGroundBody(nullptr), mMouseSpawnObject(ObjectTypes::Box),
		mMouseJoint(nullptr), mScore(0),mDevEnvironmentActive(false)
	{
	}

	void ReflectionDemo::Initialize()
	{
		// Retrieve physics engine
		mPhysicsEngine = reinterpret_cast<Box2DComponent*>(mGame->Services().GetService(Box2DComponent::TypeIdClass()));
		assert(mPhysicsEngine != nullptr);
		mPhysicsEngine->SetBodyDestroyedCallback([&](b2Body* body) {
			UNREFERENCED_PARAMETER(body);
			mShapeCount--;
		});

		// Retrieve the physics debug drawing system
		mPhysicsDebugDraw = reinterpret_cast<Box2DDebugDraw*>(mGame->Services().GetService(Box2DDebugDraw::TypeIdClass()));
		assert(mPhysicsDebugDraw != nullptr);
		mPhysicsDebugDraw->ToggleDrawingFlag(Box2DDebugDraw::DrawOptions::DrawOptionsJoints);

		mContactListener = make_unique<ContactListener>();

		mDestructionListener.SetMouseJointDestroyedCallback([&] { mMouseJoint = nullptr; });		

		// Retrieve the keyboard service
		mKeyboard = reinterpret_cast<KeyboardComponent*>(mGame->Services().GetService(KeyboardComponent::TypeIdClass()));
		assert(mKeyboard != nullptr);

		// Retrieve the mouse service
		mMouse = reinterpret_cast<MouseComponent*>(mGame->Services().GetService(MouseComponent::TypeIdClass()));
		assert(mMouse != nullptr);

		mHelpFont = make_unique<SpriteFont>(mGame->Direct3DDevice(), L"Content\\Fonts\\Arial_14_Regular.spritefont");		

		using namespace std::placeholders;
		mSpawnMethods =
		{
			{ ObjectTypes::Box, bind(&ReflectionDemo::SpawnBox, this, _1) },
			{ ObjectTypes::Circle, bind(&ReflectionDemo::SpawnCircle, this, _1) },
			{ ObjectTypes::Triangle, bind(&ReflectionDemo::SpawnTriangle, this, _1) },
			{ ObjectTypes::Stick, bind(&ReflectionDemo::SpawnStick, this, _1) },
		};

		mKeyMappings =
		{
			{ Keys::V,		[&]() { if(mDevEnvironmentActive) mPhysicsDebugDraw->SetVisible(!mPhysicsDebugDraw->Visible()); } },
			{ Keys::R,		bind(&ReflectionDemo::ResetWorld, this) }, //will break
			{ Keys::Space,	[&]() { if(mDevEnvironmentActive) SpawnObject(ObjectTypes::Box, BodySpawnPosition); } },
			{ Keys::Enter,	[&]() { if(mDevEnvironmentActive) SpawnObject(ObjectTypes::Circle, BodySpawnPosition); } },
			{ Keys::Back,	[&]() { if(mDevEnvironmentActive) SpawnObject(ObjectTypes::Triangle, BodySpawnPosition); } },
			{ Keys::Insert,	[&]() { if(mDevEnvironmentActive) SpawnObject(ObjectTypes::Bolas, BodySpawnPosition); } },
			{ Keys::K,		[&]() { if(mDevEnvironmentActive)SpawnObject(ObjectTypes::Stick, BodySpawnPosition); } },
			{ Keys::B,		[&]() { if(mDevEnvironmentActive) mPhysicsDebugDraw->ToggleDrawingFlag(Box2DDebugDraw::DrawOptions::DrawOptionsAABBs); } },
			{ Keys::C,		[&]() { if(mDevEnvironmentActive) mPhysicsDebugDraw->ToggleDrawingFlag(Box2DDebugDraw::DrawOptions::DrawOptionsCenterOfMass); } },
			{ Keys::J,		[&]() { if(mDevEnvironmentActive) mPhysicsDebugDraw->ToggleDrawingFlag(Box2DDebugDraw::DrawOptions::DrawOptionsJoints); } },
			{ Keys::Add,	bind(&ReflectionDemo::IncrementMouseSpawnObject, this) },
			{ Keys::Tab,    bind(&ReflectionDemo::ToggleDevEnvironment, this)},
		};

		mLevelObjectsDescription = {
			std::pair<ObjectTypes,XMVECTOR>(ObjectTypes::Box, {2.0f,1.0f - 20.0f, 0.0f,1.0f}),
			std::pair<ObjectTypes,XMVECTOR>(ObjectTypes::Box, {4.0f,1.0f- 20.0f, 0.0f,1.0f}),
			std::pair<ObjectTypes,XMVECTOR>(ObjectTypes::Box, {6.0f,1.0f - 20.0f, 0.0f,1.0f}),
			std::pair<ObjectTypes,XMVECTOR>(ObjectTypes::Box, {8.0f,1.0f - 20.0f, 0.0f,1.0f}),
			std::pair<ObjectTypes,XMVECTOR>(ObjectTypes::Box, {10.0f,1.0f - 20.0f, 0.0f,1.0f}),
			std::pair<ObjectTypes,XMVECTOR>(ObjectTypes::Box, {12.0f,1.0f - 20.0f, 0.0f,1.0f}),

			std::pair<ObjectTypes,XMVECTOR>(ObjectTypes::Triangle, {2.0f,3.0f - 20.0f, 0.0f,1.0f}),
			//std::pair<ObjectTypes,XMVECTOR>(ObjectTypes::Triangle, {5.0f,3.0f - 20.0f, 0.0f,1.0f}),
			std::pair<ObjectTypes,XMVECTOR>(ObjectTypes::Triangle, {7.0f,3.0f - 20.0f, 0.0f,1.0f}),
			//std::pair<ObjectTypes,XMVECTOR>(ObjectTypes::Triangle, {9.0f,3.0f - 20.0f, 0.0f,1.0f}),
			std::pair<ObjectTypes,XMVECTOR>(ObjectTypes::Triangle, {12.0f,3.0f - 20.0f, 0.0f,1.0f}),

			std::pair<ObjectTypes,XMVECTOR>(ObjectTypes::Circle, {4.25,3.5f - 20.0f, 0.0f,1.0f}),
			std::pair<ObjectTypes,XMVECTOR>(ObjectTypes::Circle, {9.75f,3.5f - 20.0f, 0.0f,1.0f}),

			std::pair<ObjectTypes,XMVECTOR>(ObjectTypes::Stick, {-0.5f,3.0f - 20.0f, 0.0f,1.0f}),
			std::pair<ObjectTypes,XMVECTOR>(ObjectTypes::Triangle, {-1.0f,5.5f - 20.0f, 0.0f,1.0f}),
			std::pair<ObjectTypes,XMVECTOR>(ObjectTypes::Stick, {-1.5f,3.0f - 20.0f, 0.0f,1.0f}),
			std::pair<ObjectTypes,XMVECTOR>(ObjectTypes::Stick, {15.0f,3.0f - 20.0f, 0.0f,1.0f}),
			std::pair<ObjectTypes,XMVECTOR>(ObjectTypes::Triangle, {15.5f,5.5f - 20.0f, 0.0f,1.0f}),
			std::pair<ObjectTypes,XMVECTOR>(ObjectTypes::Stick, {16.0f,3.0f - 20.0f, 0.0f,1.0f}),

		};

		mBoxTexture = mGame->Content().Load<Texture2D>(L"Textures\\BlockWood_beige_size64.png");
		mCatYellowTexture = mGame->Content().Load<Texture2D>(L"Textures\\CatYellow.png");
		mDogTexture = mGame->Content().Load<Texture2D>(L"Textures\\DogBig.png");
		mStickTexture = mGame->Content().Load<Texture2D>(L"Textures\\elementWood019.png");
		mTriangleTexture = mGame->Content().Load<Texture2D>(L"Textures\\Triangle.png");
		mGroundTexture = mGame->Content().Load<Texture2D>(L"Textures\\grass.png");
		mFloorTexture = mGame->Content().Load<Texture2D>(L"Textures\\Ground_dirtMud1.png");

		ResetWorld();
		
	}

	void ReflectionDemo::Update(const GameTime& gameTime)
	{
		UNREFERENCED_PARAMETER(gameTime);

		for (auto& sprite : mSprites)
		{
			sprite->Update(gameTime);
		}
		for (auto& object : mGameObjects)
		{
			if (object->IsScheduledForDestruction()) 
			{
				if (object->Tag() == BehaviorType::Breakable)
					mScore += object->As<Breakable>()->ScoreValue();

				auto itObject = find_if(begin(mGameObjects), end(mGameObjects), [object](const shared_ptr<Box2DBehavior>& obj) {
					return obj.get() == object.get();
				});
				if (itObject != mGameObjects.end())
				{
					mGameObjects.erase(itObject);
					break;
				}
			}
			else
			{
				object->Update(gameTime);
			}
		}

		if (mMouse->WasButtonPressedThisFrame(MouseButtons::Left) && mDevEnvironmentActive)
		{
			
			SpawnObjectWithMouse();
		}

		if (mMouse->WasButtonPressedThisFrame(MouseButtons::Right))
		{
			CreateMouseJoint();
		}

		if (mMouse->IsButtonHeldDown(MouseButtons::Right))
		{
			if (mMouseJoint != nullptr)
			{
				ApplyForceWithMouse();
			}
		}

		if (mMouse->WasButtonReleasedThisFrame(MouseButtons::Right))
		{
			if (mMouseJoint != nullptr)
			{
				mPhysicsEngine->ScheduleJointForDestruction(*mMouseJoint);
				mMouseJoint = nullptr;
				mDestructionListener.SetMouseJoint(nullptr);
			}
		}

		if (mMouse->WasButtonReleasedThisFrame(MouseButtons::Middle))
		{
			IncrementMouseSpawnObject();
		}

		for (const auto& keyMapping : mKeyMappings)
		{
			if (mKeyboard->WasKeyPressedThisFrame(keyMapping.first))
			{
				keyMapping.second();
			}
		}
	}

	void ReflectionDemo::Draw(const GameTime& gameTime)
	{
		UNREFERENCED_PARAMETER(gameTime);
		assert(mCamera != nullptr);

		for (auto& sprite : mSprites)
		{
			sprite->Draw(gameTime);
		}
		for (auto& objects : mGameObjects)
		{
			objects->Draw(gameTime);
		}

		wostringstream helpText;
		if (mDevEnvironmentActive) {
			helpText << HelpText;
			helpText << L"\nRigid Body Count: " << mPhysicsEngine->World().GetBodyCount();
			helpText << L"\nShape Count: " << mShapeCount;
			helpText << L"\nMouse Spawn Object: " << SpawnObjectNames.at(mMouseSpawnObject);
		}
		else {
			helpText << GameText;
		}
		helpText << L"\nScore: " << mScore;

		SpriteManager::DrawString(mHelpFont, helpText.str().c_str(), TextPosition);
	}

	void ReflectionDemo:: SpawnLevelObjects()
	{
	
		for (std::pair<ObjectTypes, XMVECTOR> pair : mLevelObjectsDescription) {
			SpawnObject(pair.first, pair.second);
			
		}
	}

	void ReflectionDemo::AddWalls()
	{
		b2Vec2 vertices[] =
		{
			{ -50.0f, -50.0f },
			{ 50.0f, -50.0f },
			{ 50.0f, 50.0f },
			{ -50.0f, 50.0f },
			{ -50.0f, -50.0f },

		};
		
		b2ChainShape chain;
		chain.CreateChain(vertices, ARRAYSIZE(vertices));

		b2BodyDef BouncyBodyDef;
		BouncyBodyDef.type = b2_staticBody;
		
		b2FixtureDef BouncyFixtureDef;
		BouncyFixtureDef.shape = &(chain);

		b2Body* body = mPhysicsEngine->World().CreateBody(&BouncyBodyDef);

		body->CreateFixture(&BouncyFixtureDef);
		
		body->SetUserData(NULL);

		mShapeCount++;
	}

	void ReflectionDemo::AddBall()
	{
		const float radius = 1.0f;
		const b2Vec2 startVelocity(0.0f, -20.0f);
		auto sprite = Box2DSprite::CreateCircle(*mGame, mCamera, mCatYellowTexture, XMFLOAT2(0.0f,5.0f), radius);
		auto bouncy = make_shared<Reflection2DGame::Bouncy>(*mGame, mCamera, sprite);
		bouncy->Initialize();
		sprite->Body()->SetType(b2BodyType::b2_kinematicBody);
		sprite->Body()->SetUserData(bouncy.get());
		sprite->Body()->SetLinearVelocity(startVelocity);
		mGameObjects.push_back(bouncy);
		mShapeCount++;
	}

	void ReflectionDemo::AddPaddle()
	{
		const XMFLOAT2 size(0.5f, 2.0f);
		auto sprite = Box2DSprite::CreateBox(*mGame, mCamera, mStickTexture, XMFLOAT2(), size);
		auto paddle = make_shared<Reflection2DGame::Paddle>(*mGame, mCamera, sprite);
		paddle->Initialize();
		sprite->Body()->SetType(b2BodyType::b2_staticBody);
		sprite->Body()->SetUserData(paddle.get());
		mGameObjects.push_back(paddle);
		mShapeCount++;
	}

	inline void ReflectionDemo::SpawnObject(ObjectTypes type, DirectX::FXMVECTOR position)
	{
		assert(type != ObjectTypes::End);
		mSpawnMethods.at(type)(position);
	}

	void ReflectionDemo::SpawnBox(FXMVECTOR position)
	{
		const XMFLOAT2 size = Vector2Helper::One;
		auto sprite = Box2DSprite::CreateBox(*mGame, mCamera, mBoxTexture, position, size);
		auto Breakable = make_shared<Reflection2DGame::Breakable>(*mGame, mCamera, sprite, 100.0f,50);
		Breakable->Initialize();
		sprite->Body()->SetType(b2BodyType::b2_kinematicBody);
		sprite->Body()->SetUserData(Breakable.get());
		mGameObjects.push_back(Breakable);
		mShapeCount++;
	}

	void ReflectionDemo::SpawnCircle(FXMVECTOR position)
	{
		const float radius = 1.0f;
		auto sprite = Box2DSprite::CreateCircle(*mGame, mCamera, mCatYellowTexture, position, radius);
		auto Breakable = make_shared<Reflection2DGame::Breakable>(*mGame, mCamera, sprite, 200.0f,100);
		Breakable->Initialize();
		sprite->Body()->SetType(b2BodyType::b2_kinematicBody);
		sprite->Body()->SetUserData(Breakable.get());
		mGameObjects.push_back(Breakable);
		mShapeCount++;
	}

	void ReflectionDemo::SpawnTriangle(FXMVECTOR position)
	{	
		const XMFLOAT2 size = Vector2Helper::One;
		auto sprite = Box2DSprite::CreateTriangle(*mGame, mCamera, mTriangleTexture, position, size);
		auto Breakable = make_shared<Reflection2DGame::Breakable>(*mGame, mCamera, sprite, 300.0f,100);
		Breakable->Initialize();
		sprite->Body()->SetType(b2BodyType::b2_kinematicBody);
		sprite->Body()->SetUserData(Breakable.get());
		mGameObjects.push_back(Breakable);
		mShapeCount++;
	}

	void ReflectionDemo::SpawnStick(FXMVECTOR position)
	{
		const XMFLOAT2 size(0.5f, 2.0f);
		auto sprite = Box2DSprite::CreateBox(*mGame, mCamera, mStickTexture, position, size);
		auto breakable = make_shared<Reflection2DGame::Breakable>(*mGame, mCamera, sprite, 500.0f,200);
		breakable->Initialize();
		sprite->Body()->SetUserData(breakable.get());
		sprite->Body()->SetType(b2BodyType::b2_kinematicBody);
		mGameObjects.push_back(breakable);
		mShapeCount++;
	}

	//Will Break
	void ReflectionDemo::ResetWorld()
	{
		mShapeCount = 0U;
		mSprites.clear();
		mGameObjects.clear();
		mPhysicsEngine->Clear();

		b2World& world = mPhysicsEngine->World();
		world.SetContactListener(mContactListener.get());
		world.SetDestructionListener(&mDestructionListener);
		SpawnLevelObjects();
		AddWalls();
		AddBall();
		AddPaddle();

	}

	void ReflectionDemo::SpawnObjectWithMouse()
	{
		SpawnObject(mMouseSpawnObject, GetMouseWorldPosition());
	}

	void ReflectionDemo::IncrementMouseSpawnObject()
	{
		ObjectTypes newSpawnObject = ObjectTypes(static_cast<int>(mMouseSpawnObject) + 1);
		if (newSpawnObject >= ObjectTypes::End)
		{
			newSpawnObject = ObjectTypes(0);
		}

		mMouseSpawnObject = newSpawnObject;
	}

	void ReflectionDemo::CreateMouseJoint()
	{
		XMVECTOR mouseWorldPosition = GetMouseWorldPosition();
		const b2Vec2 position(XMVectorGetX(mouseWorldPosition), XMVectorGetY(mouseWorldPosition));

		// Make a small box
		b2AABB aabb;
		b2Vec2 d;
		d.Set(0.001f, 0.001f);
		aabb.lowerBound = position - d;
		aabb.upperBound = position + d;

		// Query the world for overlapping shapes
		QueryCallback callback(position);
		b2World& world = mPhysicsEngine->World();
		world.QueryAABB(&callback, aabb);

		if (callback.Fixture != nullptr)
		{
			static const float forceMultiplier = 1000.0f;
			b2Body* body = callback.Fixture->GetBody();

			b2MouseJointDef mouseJointDef;
			mouseJointDef.bodyA = mGroundBody;
			mouseJointDef.bodyB = body;
			mouseJointDef.target = position;
			mouseJointDef.maxForce = forceMultiplier * body->GetMass();
			mMouseJoint = static_cast<b2MouseJoint*>(world.CreateJoint(&mouseJointDef));
			mDestructionListener.SetMouseJoint(mMouseJoint);
			body->SetAwake(true);
		}
	}

	void ReflectionDemo::ApplyForceWithMouse()
	{
		assert(mMouseJoint != nullptr);

		XMVECTOR mouseWorldPosition = GetMouseWorldPosition();
		const b2Vec2 position(XMVectorGetX(mouseWorldPosition), XMVectorGetY(mouseWorldPosition));
		mMouseJoint->SetTarget(position);
	}

	void ReflectionDemo::ToggleDevEnvironment()
	{
		mDevEnvironmentActive = !mDevEnvironmentActive;
	}

	DirectX::XMVECTOR ReflectionDemo::GetMouseWorldPosition() const
	{
		const auto& currentState = mMouse->CurrentState();
		const XMVECTOR mouseScreenPosition = XMVectorSet(static_cast<float>(currentState.x), static_cast<float>(currentState.y), 0.0f, 1.0f);
		const auto& viewport = mGame->Viewport();
		return XMVector3Unproject(mouseScreenPosition, viewport.TopLeftX, viewport.TopLeftY, viewport.Width, viewport.Height, viewport.MinDepth, viewport.MaxDepth, mCamera->ProjectionMatrix(), mCamera->ViewMatrix(), XMMatrixIdentity());
	}
#pragma region CallbackListener

	void ReflectionDemo::ContactListener::BeginContact(b2Contact * contact)
	{
		auto userDataA = contact->GetFixtureA()->GetBody()->GetUserData();
		auto userDataB = contact->GetFixtureB()->GetBody()->GetUserData();

		Box2DBehavior* behaviorA = static_cast<Box2DBehavior*>(userDataA);
		Box2DBehavior* behaviorB =  static_cast<Box2DBehavior*>(userDataB);

		if (userDataA) {
			switch (behaviorA->Tag())
			{
			case BehaviorType::Breakable:
				behaviorA->As<Breakable>()->OnContactBegin(behaviorB, contact);
			default:
				break;
			}
		}
		if (userDataB) {
			switch (behaviorB->Tag())
			{
			case BehaviorType::Breakable:
				behaviorB->As<Breakable>()->OnContactBegin(behaviorA, contact);
			case BehaviorType::Bouncy:
				behaviorB->As<Bouncy>()->OnContactBegin(behaviorA, contact);
			default:
				break;
			}
		}
	}

	void  ReflectionDemo::ContactListener::EndContact(b2Contact* contact)
	{	
		auto userDataA = contact->GetFixtureA()->GetBody()->GetUserData();
		auto userDataB = contact->GetFixtureB()->GetBody()->GetUserData();

		Box2DBehavior* behaviorA = static_cast<Box2DBehavior*>(userDataA);
		Box2DBehavior* behaviorB =  static_cast<Box2DBehavior*>(userDataB);

		if (userDataA) {
			switch (behaviorA->Tag())
			{
			case BehaviorType::Breakable:
				behaviorA->As<Breakable>()->OnContactEnd(behaviorB, contact);
			case BehaviorType::Bouncy:
				behaviorA->As<Bouncy>()->OnContactEnd(behaviorB, contact);
			default:
				break;
			}
		}
		if (userDataB) {
			switch (behaviorB->Tag())
			{
			case BehaviorType::Breakable:
				behaviorB->As<Breakable>()->OnContactEnd(behaviorA, contact);
			case BehaviorType::Bouncy:
				behaviorB->As<Bouncy>()->OnContactEnd(behaviorA, contact);
			default:
				break;
			}
		}
	}

	void  ReflectionDemo::ContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
	{
		auto userDataA = contact->GetFixtureA()->GetBody()->GetUserData();
		auto userDataB = contact->GetFixtureB()->GetBody()->GetUserData();

		Box2DBehavior* behaviorA = static_cast<Box2DBehavior*>(userDataA);
		Box2DBehavior* behaviorB = static_cast<Box2DBehavior*>(userDataB);

		if (userDataA) {
			switch (behaviorA->Tag())
			{
			case BehaviorType::Breakable:
				behaviorA->As<Breakable>()->OnContactPreSolve(behaviorB, contact, oldManifold);
			case BehaviorType::Bouncy:
				behaviorA->As<Bouncy>()->OnContactPreSolve(behaviorB, contact, oldManifold);
			default:
				break;
			}
		}
		if (userDataB) {
			switch (behaviorB->Tag())
			{
			case BehaviorType::Breakable:
				behaviorB->As<Breakable>()->OnContactPreSolve(behaviorA, contact, oldManifold);
			case BehaviorType::Bouncy:
				behaviorB->As<Bouncy>()->OnContactPreSolve(behaviorA, contact, oldManifold);
			default:
				break;
			}
		}
	}
	void  ReflectionDemo::ContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
	{
		auto userDataA = contact->GetFixtureA()->GetBody()->GetUserData();
		auto userDataB = contact->GetFixtureB()->GetBody()->GetUserData();

		Box2DBehavior* behaviorA = static_cast<Box2DBehavior*>(userDataA);
		Box2DBehavior* behaviorB = static_cast<Box2DBehavior*>(userDataB);
		

		if (userDataA) {
			switch (behaviorA->Tag())
			{
			case BehaviorType::Breakable:
				behaviorA->As<Breakable>()->OnContactPostSolve(behaviorB, contact, impulse);
			case BehaviorType::Bouncy:
				behaviorA->As<Bouncy>()->OnContactPostSolve(behaviorB, contact, impulse);
			default:
				break;
			}
		}
		if (userDataB) {
			switch (behaviorB->Tag())
			{
			case BehaviorType::Breakable:
				behaviorB->As<Breakable>()->OnContactPostSolve(behaviorA, contact, impulse);
			case BehaviorType::Bouncy:
				behaviorB->As<Bouncy>()->OnContactPostSolve(behaviorA, contact, impulse);
			default:
				break;
			}
		}
	}

#pragma endregion

#pragma region QueryCallback

	ReflectionDemo::QueryCallback::QueryCallback(const b2Vec2& point) :
		Point(point), Fixture(nullptr)
	{
	}

	bool ReflectionDemo::QueryCallback::ReportFixture(b2Fixture* fixture)
	{
		const b2Body* body = fixture->GetBody();
		if (body->GetType() == b2_dynamicBody)
		{
			bool inside = fixture->TestPoint(Point);
			if (inside)
			{
				Fixture = fixture;

				// We are done, terminate the query.
				return false;
			}
		}

		// Continue the query.
		return true;
	}

#pragma endregion

#pragma region DestructionListener

	ReflectionDemo::DestructionListener::DestructionListener() :
		mMouseJoint(nullptr)
	{
	}

	void ReflectionDemo::DestructionListener::SetMouseJoint(b2MouseJoint* mouseJoint)
	{
		mMouseJoint = mouseJoint;
	}

	void ReflectionDemo::DestructionListener::SetMouseJointDestroyedCallback(std::function<void()> callback)
	{
		mMouseJointDestroyedCallback = callback;
	}

	void ReflectionDemo::DestructionListener::SayGoodbye(b2Joint* joint)
	{
		if (joint == mMouseJoint)
		{
			mMouseJoint = nullptr;
			if (mMouseJointDestroyedCallback != nullptr)
			{
				mMouseJointDestroyedCallback();
			}
		}
	}

#pragma endregion

}
