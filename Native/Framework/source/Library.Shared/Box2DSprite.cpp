#include "pch.h"
#include "Box2DSprite.h"

using namespace std;
using namespace DirectX;

namespace Library
{
	RTTI_DEFINITIONS(Box2DSprite)

	Box2DSprite::Box2DSprite(Game& game, const shared_ptr<Camera>& camera, const shared_ptr<Texture2D>& texture, const XMFLOAT2& position) :
		Sprite(game, camera, texture)
	{
		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		bodyDef.userData = this;
		bodyDef.position.Set(position.x, position.y);

		CreateBody(bodyDef);
	}

	Box2DSprite::Box2DSprite(Game& game, const shared_ptr<Camera>& camera, const shared_ptr<Texture2D>& texture, const b2BodyDef& bodyDef) :
		Sprite(game, camera, texture)
	{
		CreateBody(bodyDef);
		mBody->SetUserData(this);
	}

	Box2DSprite::~Box2DSprite()
	{
		if (mGame->IsShuttingDown() == false)
		{
			auto physicsEngine = reinterpret_cast<Box2DComponent*>(mGame->Services().GetService(Box2DComponent::TypeIdClass()));
			assert(physicsEngine != nullptr);
			physicsEngine->BuryBody(*mBody);
		}
	}

	b2Body* Box2DSprite::Body() const
	{
		return mBody;
	}

	void Box2DSprite::Initialize()
	{		
		Sprite::Initialize();
		UpdateTransformFromBody();
	}

	void Box2DSprite::Update(const GameTime& gameTime)
	{
		UNREFERENCED_PARAMETER(gameTime);
		UpdateTransformFromBody();
	}

	void Box2DSprite::CreateBody(const b2BodyDef& bodyDef)
	{
		// Retrieve physics engine
		auto physicsEngine = reinterpret_cast<Box2DComponent*>(mGame->Services().GetService(Box2DComponent::TypeIdClass()));
		assert(physicsEngine != nullptr);

		b2World& world = physicsEngine->World();
		mBody = world.CreateBody(&bodyDef);

		b2PolygonShape shape;
		const auto& scale = mTransform.Scale();
		shape.SetAsBox(scale.x, scale.y);

		// TODO: density, friction...
		b2FixtureDef fixtureDef;
		fixtureDef.shape = &shape;
		fixtureDef.density = 1.0f;
		fixtureDef.friction = 0.3f;
		mBody->CreateFixture(&fixtureDef);
	}

	void Box2DSprite::UpdateTransformFromBody()
	{
		const auto& position = mBody->GetPosition();
		mTransform.SetPosition(position.x, position.y);
		mTransform.SetRotation(mBody->GetAngle());
	}
}

