#include "pch.h"
#include "PhysicsDemo.h"

using namespace std;
using namespace Library;
using namespace DirectX;

namespace Box2DDemo
{
	const XMFLOAT2 PhysicsDemo::TextPosition = { 0.0f, 42.0f };
	const wstring PhysicsDemo::HelpText = L"Reset (R)\nAdd Box (Space)\nAdd Circle (Enter)\nAdd Triangle (Backspace)\nAdd Bolas (Insert)\nToggle AABBs (B)\nToggle Center of Mass (C)\nToggle Joints (J)\nSpawn w/ Mouse (Left Mouse Button)\nChange Mouse Spawn Object (+)\nGrab Object (Right Mouse Button)";
	const XMVECTORF32 PhysicsDemo::BodySpawnPosition = { 0.0f, 8.0f, 0.0f, 1.0f };
	const map<PhysicsDemo::ObjectTypes, wstring> PhysicsDemo::SpawnObjectNames =
	{
		{ ObjectTypes::Box, L"Box" },
		{ ObjectTypes::Circle, L"Circle" },
		{ ObjectTypes::Triangle, L"Triangle" },
		{ ObjectTypes::Bolas, L"Bolas" },
		{ ObjectTypes::End, L"" },
	};

	PhysicsDemo::PhysicsDemo(Game& game, const shared_ptr<Camera>& camera) :
		DrawableGameComponent(game, camera),
		mPhysicsEngine(nullptr), mPhysicsDebugDraw(nullptr), mKeyboard(nullptr),
		mShapeCount(0U), mGroundBody(nullptr), mMouseSpawnObject(ObjectTypes::Circle),
		mMouseJoint(nullptr)
	{
	}

	void PhysicsDemo::Initialize()
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

		mContactListener = make_unique<ContactListener>(mPhysicsEngine);		
		mDestructionListener.SetMouseJointDestroyedCallback([&] { mMouseJoint = nullptr; });
		ResetWorld();

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
			{ ObjectTypes::Box, bind(&PhysicsDemo::SpawnBox, this, _1) },
			{ ObjectTypes::Circle, bind(&PhysicsDemo::SpawnCircle, this, _1) },
			{ ObjectTypes::Triangle, bind(&PhysicsDemo::SpawnTriangle, this, _1) },
			{ ObjectTypes::Bolas, bind(&PhysicsDemo::SpawnBolas, this, _1) },
		};

		mKeyMappings =
		{
			{ Keys::R,		bind(&PhysicsDemo::ResetWorld, this) },
			{ Keys::Space,	[&]() { SpawnObject(ObjectTypes::Box, BodySpawnPosition); } },
			{ Keys::Enter,	[&]() { SpawnObject(ObjectTypes::Circle, BodySpawnPosition); } },
			{ Keys::Back,	[&]() { SpawnObject(ObjectTypes::Triangle, BodySpawnPosition); } },
			{ Keys::Insert,	[&]() { SpawnObject(ObjectTypes::Bolas, BodySpawnPosition); } },
			{ Keys::B,		[&]() { mPhysicsDebugDraw->ToggleDrawingFlag(Box2DDebugDraw::DrawOptions::DrawOptionsAABBs); } },
			{ Keys::C,		[&]() { mPhysicsDebugDraw->ToggleDrawingFlag(Box2DDebugDraw::DrawOptions::DrawOptionsCenterOfMass); } },
			{ Keys::J,		[&]() { mPhysicsDebugDraw->ToggleDrawingFlag(Box2DDebugDraw::DrawOptions::DrawOptionsJoints); } },
			{ Keys::Add,	bind(&PhysicsDemo::IncrementMouseSpawnObject, this) },
		};
	}

	void PhysicsDemo::Update(const GameTime& gameTime)
	{
		UNREFERENCED_PARAMETER(gameTime);

		for (const auto& keyMapping : mKeyMappings)
		{
			if (mKeyboard->WasKeyPressedThisFrame(keyMapping.first))
			{
				keyMapping.second();
			}
		}

		if (mMouse->WasButtonPressedThisFrame(MouseButtons::Left))
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
	}

	void PhysicsDemo::Draw(const GameTime& gameTime)
	{
		UNREFERENCED_PARAMETER(gameTime);

		wostringstream helpText;
		helpText << HelpText << L"\nRigid Body Count: " << mPhysicsEngine->World().GetBodyCount();
		helpText << L"\nShape Count: " << mShapeCount;
		helpText << L"\nMouse Spawn Object: " << SpawnObjectNames.at(mMouseSpawnObject);
		SpriteManager::DrawString(mHelpFont, helpText.str().c_str(), TextPosition);
	}

	void PhysicsDemo::AddGround()
	{
		b2World& world = mPhysicsEngine->World();

		// Create the ground body, shape, and fixture
		b2BodyDef groundBodyDef;
		groundBodyDef.position.Set(0.0f, -10.0f);
		mGroundBody = world.CreateBody(&groundBodyDef);

		b2PolygonShape groundBox;
		groundBox.SetAsBox(10.0f, 1.0f);
		mGroundBody->CreateFixture(&groundBox, 0.0f);
		mShapeCount++;
		
		// Create a pass-through floor (e.g. water/lava pool)
		b2Vec2 v1(-1000.0f, -10.0f);
		b2Vec2 v2(1000.0f, -10.0f);
		b2EdgeShape floorShape;
		floorShape.Set(v1, v2);

		b2FixtureDef floorFixtureDef;
		floorFixtureDef.shape = &floorShape;
		floorFixtureDef.isSensor = true;
		b2Fixture* floorFixture = mGroundBody->CreateFixture(&floorFixtureDef);
		mContactListener->SetFloorFixture(floorFixture);
		mShapeCount++;
	}

	void PhysicsDemo::AddEdge()
	{
		b2Vec2 v1(-10.0f, 0.0f);
		b2Vec2 v2(-10.0f, 10.0f);
		b2EdgeShape edge;
		edge.Set(v1, v2);

		mGroundBody->CreateFixture(&edge, 0.0f);
		mShapeCount++;
	}

	void PhysicsDemo::AddChain()
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

	inline void PhysicsDemo::SpawnObject(ObjectTypes type, DirectX::FXMVECTOR position)
	{
		assert(type != ObjectTypes::End);
		mSpawnMethods.at(type)(position);
	}

	void PhysicsDemo::SpawnBox(FXMVECTOR position)
	{
		b2World& world = mPhysicsEngine->World();

		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		bodyDef.position.Set(XMVectorGetX(position), XMVectorGetY(position));
		b2Body* body = world.CreateBody(&bodyDef);

		b2PolygonShape shape;
		shape.SetAsBox(1.0f, 1.0f);

		b2FixtureDef fixtureDef;
		fixtureDef.shape = &shape;
		fixtureDef.density = 1.0f;
		fixtureDef.friction = 0.3f;
		body->CreateFixture(&fixtureDef);
		mShapeCount++;
	}

	void PhysicsDemo::SpawnCircle(FXMVECTOR position)
	{
		b2World& world = mPhysicsEngine->World();

		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		bodyDef.position.Set(XMVectorGetX(position), XMVectorGetY(position));
		b2Body* body = world.CreateBody(&bodyDef);

		b2CircleShape shape;
		shape.m_radius = 1.0f;

		b2FixtureDef fixtureDef;
		fixtureDef.shape = &shape;
		fixtureDef.density = 1.0f;
		fixtureDef.friction = 0.3f;
		body->CreateFixture(&fixtureDef);
		mShapeCount++;
	}

	void PhysicsDemo::SpawnTriangle(FXMVECTOR position)
	{
		b2World& world = mPhysicsEngine->World();

		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		bodyDef.position.Set(XMVectorGetX(position), XMVectorGetY(position));
		b2Body* body = world.CreateBody(&bodyDef);

		b2Vec2 vertices[] =
		{	
			{ +1.0f, -1.0f },
			{ +0.0f, +1.0f },			
			{ -1.0f, -1.0f },
		};

		b2PolygonShape shape;
		shape.Set(vertices, ARRAYSIZE(vertices));

		b2FixtureDef fixtureDef;
		fixtureDef.shape = &shape;
		fixtureDef.density = 1.0f;
		fixtureDef.friction = 0.3f;
		body->CreateFixture(&fixtureDef);
		mShapeCount++;
	}

	void PhysicsDemo::SpawnBolas(FXMVECTOR position)
	{
		b2World& world = mPhysicsEngine->World();

		b2CircleShape ballShape;
		ballShape.m_radius = 0.75f;

		b2FixtureDef fixtureDef;
		fixtureDef.shape = &ballShape;
		fixtureDef.density = 1.0f;
		fixtureDef.friction = 0.3f;
				
		// Create left-side ball
		static const float horizontalOffset = 2.0f;
		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		bodyDef.position.Set(XMVectorGetX(position) - horizontalOffset, XMVectorGetY(position));
		b2Body* leftBody = world.CreateBody(&bodyDef);		
		leftBody->CreateFixture(&fixtureDef);
		mShapeCount++;

		// Create right-side ball
		bodyDef.position.Set(XMVectorGetX(position) + horizontalOffset, XMVectorGetY(position));
		b2Body* rightBody = world.CreateBody(&bodyDef);
		rightBody->CreateFixture(&fixtureDef);
		mShapeCount++;

		// Create tether
		b2RopeJointDef jointDef;
		jointDef.bodyA = leftBody;
		jointDef.bodyB = rightBody;
		jointDef.maxLength = b2Distance(leftBody->GetPosition(), rightBody->GetPosition());
		jointDef.localAnchorA.SetZero();
		jointDef.localAnchorB.SetZero();
		jointDef.collideConnected = true;
		world.CreateJoint(&jointDef);
	}

	void PhysicsDemo::ResetWorld()
	{
		mShapeCount = 0U;
		mPhysicsEngine->Clear();
		mContactListener->SetFloorFixture(nullptr);
		AddGround();
		AddEdge();
		AddChain();
		b2World& world = mPhysicsEngine->World();
		world.SetContactListener(mContactListener.get());
		world.SetDestructionListener(&mDestructionListener);
	}

	void PhysicsDemo::SpawnObjectWithMouse()
	{		
		SpawnObject(mMouseSpawnObject, GetMouseWorldPosition());
	}

	void PhysicsDemo::IncrementMouseSpawnObject()
	{
		ObjectTypes newSpawnObject = ObjectTypes(static_cast<int>(mMouseSpawnObject) + 1);
		if (newSpawnObject >= ObjectTypes::End)
		{
			newSpawnObject = ObjectTypes(0);
		}

		mMouseSpawnObject = newSpawnObject;
	}

	void PhysicsDemo::CreateMouseJoint()
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

	void PhysicsDemo::ApplyForceWithMouse()
	{
		assert(mMouseJoint != nullptr);

		XMVECTOR mouseWorldPosition = GetMouseWorldPosition();
		const b2Vec2 position(XMVectorGetX(mouseWorldPosition), XMVectorGetY(mouseWorldPosition));
		mMouseJoint->SetTarget(position);
	}

	DirectX::XMVECTOR PhysicsDemo::GetMouseWorldPosition() const
	{
		const auto& currentState = mMouse->CurrentState();
		const XMVECTOR mouseScreenPosition = XMVectorSet(static_cast<float>(currentState.x), static_cast<float>(currentState.y), 0.0f, 1.0f);
		const auto& viewport = mGame->Viewport();
		return XMVector3Unproject(mouseScreenPosition, viewport.TopLeftX, viewport.TopLeftY, viewport.Width, viewport.Height, viewport.MinDepth, viewport.MaxDepth, mCamera->ProjectionMatrix(), mCamera->ViewMatrix(), XMMatrixIdentity());
	}

#pragma region ContactListener

	PhysicsDemo::ContactListener::ContactListener(Box2DComponent* physicsEngine) :
		mPhysicsEngine(physicsEngine), mFloorFixture(nullptr)
	{
		assert(mPhysicsEngine != nullptr);
	}

	const b2Fixture* PhysicsDemo::ContactListener::FloorFixture() const
	{
		return mFloorFixture;
	}

	void PhysicsDemo::ContactListener::SetFloorFixture(b2Fixture* fixture)
	{
		mFloorFixture = fixture;
	}

	void PhysicsDemo::ContactListener::EndContact(b2Contact* contact)
	{
		b2Fixture* fallingFixture = (contact->GetFixtureA() == mFloorFixture ? contact->GetFixtureB() : (contact->GetFixtureB() == mFloorFixture ? contact->GetFixtureA() : nullptr));
		if (fallingFixture != nullptr)
		{
			b2Body* fallingBody = fallingFixture->GetBody();
			mPhysicsEngine->BuryBody(*fallingBody);
		}
	}

#pragma endregion

#pragma region QueryCallback

	PhysicsDemo::QueryCallback::QueryCallback(const b2Vec2& point) :
		Point(point), Fixture(nullptr)
	{
	}

	bool PhysicsDemo::QueryCallback::ReportFixture(b2Fixture* fixture)
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
	
	PhysicsDemo::DestructionListener::DestructionListener() :
		mMouseJoint(nullptr)
	{
	}

	void PhysicsDemo::DestructionListener::SetMouseJoint(b2MouseJoint* mouseJoint)
	{
		mMouseJoint = mouseJoint;
	}

	void PhysicsDemo::DestructionListener::SetMouseJointDestroyedCallback(std::function<void()> callback)
	{
		mMouseJointDestroyedCallback = callback;
	}

	void PhysicsDemo::DestructionListener::SayGoodbye(b2Joint* joint)
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