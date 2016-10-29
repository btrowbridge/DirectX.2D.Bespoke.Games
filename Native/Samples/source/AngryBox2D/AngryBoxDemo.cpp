#include "pch.h"
#include "AngryBoxDemo.h"

using namespace std;
using namespace Library;
using namespace DirectX;

namespace AngryBox2DGame
{
	const XMFLOAT2 AngryBoxDemo::TextPosition = { 0.0f, 42.0f };
	const std::wstring AngryBoxDemo::HelpText = L"Toggle Dev Environment (Tab) \nAdd Box (Space)\nAdd Circle (Enter)\nAdd Triangle (Backspace)\nAdd Stick (K)\nAdd Bolas (Insert)\nToggle Debug Draw (V)\nToggle AABBs (B)\nToggle Center of Mass (C)\nToggle Joints (J)\nSpawn w/ Mouse (Left Mouse Button)\nChange Mouse Spawn Object (+)\nGrab Object (Right Mouse Button)";
	const std::wstring AngryBoxDemo::GameText = L"Toggle Dev Environment (Tab) \nGrab Ammo Bird (Right Mouse Button)";
	const XMVECTORF32 AngryBoxDemo::BodySpawnPosition = { 0.0f, 8.0f, 0.0f, 1.0f };
	const map<AngryBoxDemo::ObjectTypes, std::wstring> AngryBoxDemo::SpawnObjectNames =
	{
		{ ObjectTypes::Box, L"Box" },
		{ ObjectTypes::Circle, L"Circle" },
		{ ObjectTypes::Triangle, L"Triangle" },
		{ ObjectTypes::Bolas, L"Bolas" },
		{ ObjectTypes::Stick, L"Stick" },
		{ ObjectTypes::End, L"" },
	};


	AngryBoxDemo::AngryBoxDemo(Game& game, const shared_ptr<Camera>& camera) :
		DrawableGameComponent(game, camera),
		mPhysicsEngine(nullptr), mPhysicsDebugDraw(nullptr), mKeyboard(nullptr),
		mShapeCount(0U), mGroundBody(nullptr), mMouseSpawnObject(ObjectTypes::Box),
		mMouseJoint(nullptr), mScore(0),mDevEnvironmentActive(false)
	{
	}

	void AngryBoxDemo::Initialize()
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
			{ ObjectTypes::Box, bind(&AngryBoxDemo::SpawnBox, this, _1) },
			{ ObjectTypes::Circle, bind(&AngryBoxDemo::SpawnCircle, this, _1) },
			{ ObjectTypes::Triangle, bind(&AngryBoxDemo::SpawnTriangle, this, _1) },
			{ ObjectTypes::Bolas, bind(&AngryBoxDemo::SpawnBolas, this, _1) },
			{ ObjectTypes::Stick, bind(&AngryBoxDemo::SpawnStick, this, _1) },
		};

		mKeyMappings =
		{
			{ Keys::V,		[&]() { if(mDevEnvironmentActive) mPhysicsDebugDraw->SetVisible(!mPhysicsDebugDraw->Visible()); } },
			/*{ Keys::R,		bind(&AngryBoxDemo::ResetWorld, this) }, *///will break
			{ Keys::Space,	[&]() { if(mDevEnvironmentActive) SpawnObject(ObjectTypes::Box, BodySpawnPosition); } },
			{ Keys::Enter,	[&]() { if(mDevEnvironmentActive) SpawnObject(ObjectTypes::Circle, BodySpawnPosition); } },
			{ Keys::Back,	[&]() { if(mDevEnvironmentActive) SpawnObject(ObjectTypes::Triangle, BodySpawnPosition); } },
			{ Keys::Insert,	[&]() { if(mDevEnvironmentActive) SpawnObject(ObjectTypes::Bolas, BodySpawnPosition); } },
			{ Keys::K,		[&]() { if(mDevEnvironmentActive)SpawnObject(ObjectTypes::Stick, BodySpawnPosition); } },
			{ Keys::B,		[&]() { if(mDevEnvironmentActive) mPhysicsDebugDraw->ToggleDrawingFlag(Box2DDebugDraw::DrawOptions::DrawOptionsAABBs); } },
			{ Keys::C,		[&]() { if(mDevEnvironmentActive) mPhysicsDebugDraw->ToggleDrawingFlag(Box2DDebugDraw::DrawOptions::DrawOptionsCenterOfMass); } },
			{ Keys::J,		[&]() { if(mDevEnvironmentActive) mPhysicsDebugDraw->ToggleDrawingFlag(Box2DDebugDraw::DrawOptions::DrawOptionsJoints); } },
			{ Keys::Add,	bind(&AngryBoxDemo::IncrementMouseSpawnObject, this) },
			{ Keys::Tab,    bind(&AngryBoxDemo::ToggleDevEnvironment, this)},
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
			std::pair<ObjectTypes,XMVECTOR>(ObjectTypes::Bolas, {7.0f,3.5f - 20.0f, 0.0f,1.0f}),
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

	void AngryBoxDemo::Update(const GameTime& gameTime)
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

	void AngryBoxDemo::Draw(const GameTime& gameTime)
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

	void AngryBoxDemo::AddAmmo()
	{
		
		FXMVECTOR position = { mSlingTarget.x, mSlingTarget.y, 0.0f,1.0f };
		const float radius = 1.0f;
		auto sprite = Box2DSprite::CreateCircle(*mGame, mCamera, mCatYellowTexture, position, radius);
		mAmmo = make_shared<Ammunition>(*mGame, mCamera, sprite, mGroundBody, mSlingTarget);
		sprite->Body()->SetUserData(mAmmo.get());
		sprite->Body()->GetFixtureList()->SetFriction(100.0f);
		mAmmo->Initialize();
		mGameObjects.push_back(mAmmo);
		
		mShapeCount++;
	}

	void AngryBoxDemo:: SpawnLevelObjects()
	{
	
		for (std::pair<ObjectTypes, XMVECTOR> pair : mLevelObjectsDescription) {
			SpawnObject(pair.first, pair.second);
			
		}
	}

	void AngryBoxDemo::AddBarrier()
	{
		auto size = XMFLOAT2(100.0f, 100.0f);
		auto position = XMFLOAT2(0.0f, 0.0f);

		auto sprite = Box2DSprite::CreateBox(*mGame, mCamera, mFloorTexture, position, size);
		sprite->SetVisible(false);
		sprite->Body()->GetFixtureList()->SetSensor(true);
		sprite->Body()->SetType(b2_staticBody);
		auto barrier = make_shared<Barrier>(*mGame, mCamera, sprite);
		sprite->Body()->SetUserData(barrier.get());

		mGameObjects.push_back(barrier);

		mShapeCount++;
	}

	void AngryBoxDemo::AddGround()
	{
		const XMFLOAT2 position(0.0f, -20.0f);
		const XMFLOAT2 size(100.0f, 1.0f);

		b2BodyDef bodyDef;
		bodyDef.type = b2_staticBody;
		bodyDef.position.Set(position.x, position.y);

		b2PolygonShape shape;
		shape.SetAsBox(size.x, size.y);

		auto sprite = make_shared<Box2DSprite>(*mGame, mCamera, mGroundTexture, Box2DSpritePolygonDef(bodyDef, shape), size);
		
		mGroundBody = sprite->Body();
		mShapeCount++;		

		sprite->Initialize();
		mSprites.push_back(sprite);
	}

	void AngryBoxDemo::AddEdge()
	{
		b2Vec2 v1(-10.0f, 0.0f);
		b2Vec2 v2(-10.0f, 10.0f);
		b2EdgeShape edge;
		edge.Set(v1, v2);
		
		b2FixtureDef fixDef;
		fixDef.shape = &(edge);
		fixDef.isSensor = true;

		mGroundBody->CreateFixture(&fixDef);
		
		mSlingTarget = v2 + mGroundBody->GetPosition();
		mShapeCount++;
	}

	void AngryBoxDemo::AddChain()
	{
		b2Vec2 vertices[] =
		{
			{ -2.5f, 10.0f },
			{ 0.0f, 12.0f },
			{ 2.5f, 10.0f },
		};
		
		b2ChainShape chain;
		chain.CreateChain(vertices, ARRAYSIZE(vertices));
		mGroundBody->CreateFixture(&chain, 0.0f);
		mShapeCount++;
	}

	inline void AngryBoxDemo::SpawnObject(ObjectTypes type, DirectX::FXMVECTOR position)
	{
		assert(type != ObjectTypes::End);
		mSpawnMethods.at(type)(position);
	}

	void AngryBoxDemo::SpawnBox(FXMVECTOR position)
	{
		const XMFLOAT2 size = Vector2Helper::One;
		auto sprite = Box2DSprite::CreateBox(*mGame, mCamera, mBoxTexture, position, size);
		auto Breakable = make_shared<AngryBox2DGame::Breakable>(*mGame, mCamera, sprite, 100.0f,50);
		Breakable->Initialize();
		sprite->Body()->SetUserData(Breakable.get());
		mGameObjects.push_back(Breakable);
		mShapeCount++;
	}

	void AngryBoxDemo::SpawnCircle(FXMVECTOR position)
	{
		const float radius = 1.0f;
		auto sprite = Box2DSprite::CreateCircle(*mGame, mCamera, mCatYellowTexture, position, radius);
		auto Breakable = make_shared<AngryBox2DGame::Breakable>(*mGame, mCamera, sprite, 200.0f,100);
		Breakable->Initialize();
		sprite->Body()->SetUserData(Breakable.get());
		mGameObjects.push_back(Breakable);
		mShapeCount++;
	}

	void AngryBoxDemo::SpawnTriangle(FXMVECTOR position)
	{	
		const XMFLOAT2 size = Vector2Helper::One;
		auto sprite = Box2DSprite::CreateTriangle(*mGame, mCamera, mTriangleTexture, position, size);
		auto Breakable = make_shared<AngryBox2DGame::Breakable>(*mGame, mCamera, sprite, 300.0f,100);
		Breakable->Initialize();
		sprite->Body()->SetUserData(Breakable.get());
		mGameObjects.push_back(Breakable);
		mShapeCount++;
	}

	void AngryBoxDemo::SpawnBolas(FXMVECTOR position)
	{
		const float radius = 0.75f;
		static const float horizontalOffset = 2.0f;
		
		// Create left-side ball
		const XMFLOAT2 leftSidePosition = XMFLOAT2(XMVectorGetX(position) - horizontalOffset, XMVectorGetY(position));
		auto leftSprite = Box2DSprite::CreateCircle(*mGame, mCamera, mDogTexture, leftSidePosition, radius);
		leftSprite->Initialize();
		auto breakableLeft = make_shared<AngryBox2DGame::Breakable>(*mGame, mCamera, leftSprite, 500.0f,100);
		breakableLeft->Initialize();
		leftSprite->Body()->SetUserData(breakableLeft.get());
		mGameObjects.push_back(breakableLeft);
		mShapeCount++;


		// Create right-side ball
		const XMFLOAT2 rightSidePosition = XMFLOAT2(XMVectorGetX(position) + horizontalOffset, XMVectorGetY(position));
		auto rightSprite = Box2DSprite::CreateCircle(*mGame, mCamera, mDogTexture, rightSidePosition, radius);
		auto breakableRight = make_shared<AngryBox2DGame::Breakable>(*mGame, mCamera, rightSprite, 500.0f,100);
		breakableRight->Initialize();
		rightSprite->Body()->SetUserData(breakableRight.get());
		mGameObjects.push_back(breakableRight);
		mShapeCount++;

		// Create tether
		b2RopeJointDef jointDef;
		jointDef.bodyA = leftSprite->Body();
		jointDef.bodyB = rightSprite->Body();
		jointDef.maxLength = b2Distance(leftSprite->Body()->GetPosition(), rightSprite->Body()->GetPosition());
		jointDef.localAnchorA.SetZero();
		jointDef.localAnchorB.SetZero();
		jointDef.collideConnected = true;
		mPhysicsEngine->World().CreateJoint(&jointDef);
	}

	void AngryBoxDemo::SpawnStick(FXMVECTOR position)
	{
		const XMFLOAT2 size(0.5f, 2.0f);
		auto sprite = Box2DSprite::CreateBox(*mGame, mCamera, mStickTexture, position, size);
		auto breakable = make_shared<AngryBox2DGame::Breakable>(*mGame, mCamera, sprite, 500.0f,200);
		breakable->Initialize();
		sprite->Body()->SetUserData(breakable.get());
		mGameObjects.push_back(breakable);
		mShapeCount++;
	}

	//Will Break
	void AngryBoxDemo::ResetWorld()
	{
		mShapeCount = 0U;
		mSprites.clear();
		mGameObjects.clear();
		mPhysicsEngine->Clear();
		AddGround();
		AddEdge();
		b2World& world = mPhysicsEngine->World();
		world.SetContactListener(mContactListener.get());
		world.SetDestructionListener(&mDestructionListener);
		SpawnLevelObjects();
		AddAmmo();
		AddBarrier();

	}

	void AngryBoxDemo::SpawnObjectWithMouse()
	{
		SpawnObject(mMouseSpawnObject, GetMouseWorldPosition());
	}

	void AngryBoxDemo::IncrementMouseSpawnObject()
	{
		ObjectTypes newSpawnObject = ObjectTypes(static_cast<int>(mMouseSpawnObject) + 1);
		if (newSpawnObject >= ObjectTypes::End)
		{
			newSpawnObject = ObjectTypes(0);
		}

		mMouseSpawnObject = newSpawnObject;
	}

	void AngryBoxDemo::CreateMouseJoint()
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

			if (!mDevEnvironmentActive && body != mAmmo->Sprite()->Body())
				return;

			if (body == mAmmo->Sprite()->Body() && !mAmmo->Ready())
				return;

			b2MouseJointDef mouseJointDef;
			mouseJointDef.bodyA = mGroundBody;
			mouseJointDef.bodyB = body;
			mouseJointDef.target = position;
			mouseJointDef.maxForce = forceMultiplier * body->GetMass();
			mMouseJoint = static_cast<b2MouseJoint*>(world.CreateJoint(&mouseJointDef));
			mDestructionListener.SetMouseJoint(mMouseJoint);
			body->SetAwake(true);

			if (body->GetUserData()) {
				auto bahavior = static_cast<Box2DBehavior*>(body->GetUserData());
				switch (bahavior->Tag()) {
				case BehaviorType::Ammunition:
					bahavior->As<Ammunition>()->OnClick();
				default:
					break;
				}
			}
		}
	}

	void AngryBoxDemo::ApplyForceWithMouse()
	{
		assert(mMouseJoint != nullptr);

		XMVECTOR mouseWorldPosition = GetMouseWorldPosition();
		const b2Vec2 position(XMVectorGetX(mouseWorldPosition), XMVectorGetY(mouseWorldPosition));
		mMouseJoint->SetTarget(position);
	}

	void AngryBoxDemo::ToggleDevEnvironment()
	{
		mDevEnvironmentActive = !mDevEnvironmentActive;
	}

	DirectX::XMVECTOR AngryBoxDemo::GetMouseWorldPosition() const
	{
		const auto& currentState = mMouse->CurrentState();
		const XMVECTOR mouseScreenPosition = XMVectorSet(static_cast<float>(currentState.x), static_cast<float>(currentState.y), 0.0f, 1.0f);
		const auto& viewport = mGame->Viewport();
		return XMVector3Unproject(mouseScreenPosition, viewport.TopLeftX, viewport.TopLeftY, viewport.Width, viewport.Height, viewport.MinDepth, viewport.MaxDepth, mCamera->ProjectionMatrix(), mCamera->ViewMatrix(), XMMatrixIdentity());
	}
#pragma region CallbackListener

	void AngryBoxDemo::ContactListener::BeginContact(b2Contact * contact)
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
			default:
				break;
			}
		}
	}

	void  AngryBoxDemo::ContactListener::EndContact(b2Contact* contact)
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
			default:
				break;
			}
		}
		if (userDataB) {
			switch (behaviorB->Tag())
			{
			case BehaviorType::Breakable:
				behaviorB->As<Breakable>()->OnContactEnd(behaviorA, contact);
			default:
				break;
			}
		}
	}

	void  AngryBoxDemo::ContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
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
			default:
				break;
			}
		}
		if (userDataB) {
			switch (behaviorB->Tag())
			{
			case BehaviorType::Breakable:
				behaviorB->As<Breakable>()->OnContactPreSolve(behaviorA, contact, oldManifold);
			default:
				break;
			}
		}
	}
	void  AngryBoxDemo::ContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
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
			default:
				break;
			}
		}
		if (userDataB) {
			switch (behaviorB->Tag())
			{
			case BehaviorType::Breakable:
				behaviorB->As<Breakable>()->OnContactPostSolve(behaviorA, contact, impulse);
			default:
				break;
			}
		}
	}

#pragma endregion

#pragma region QueryCallback

	AngryBoxDemo::QueryCallback::QueryCallback(const b2Vec2& point) :
		Point(point), Fixture(nullptr)
	{
	}

	bool AngryBoxDemo::QueryCallback::ReportFixture(b2Fixture* fixture)
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

	AngryBoxDemo::DestructionListener::DestructionListener() :
		mMouseJoint(nullptr)
	{
	}

	void AngryBoxDemo::DestructionListener::SetMouseJoint(b2MouseJoint* mouseJoint)
	{
		mMouseJoint = mouseJoint;
	}

	void AngryBoxDemo::DestructionListener::SetMouseJointDestroyedCallback(std::function<void()> callback)
	{
		mMouseJointDestroyedCallback = callback;
	}

	void AngryBoxDemo::DestructionListener::SayGoodbye(b2Joint* joint)
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
