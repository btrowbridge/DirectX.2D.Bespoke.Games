#include "pch.h"
#include "Box2DSprite.h"

using namespace std;
using namespace DirectX;

namespace Library
{
#pragma region Box2DSpriteDef

	const float Box2DSpriteDef::DefaultFriction = 0.3f;
	const float Box2DSpriteDef::DefaultRestitution = 0.0f;
	const float Box2DSpriteDef::DefaultDensity = 1.0f;
	
	Box2DSpriteDef::Box2DSpriteDef(const b2BodyDef& bodyDef, const float friction, const float restitution, const float density, const bool isSensor) :
		BodyDef(bodyDef), Friction(friction), Restitution(restitution), Density(density), IsSensor(isSensor)
	{
	}

	Box2DSpriteCircleDef::Box2DSpriteCircleDef(const b2BodyDef& bodyDef, const b2CircleShape& shape, const float friction, const float restitution, const float density, const bool isSensor) :
		Box2DSpriteDef(bodyDef, friction, restitution, density, isSensor),
		mShape(shape)
	{
	}

	const b2CircleShape& Box2DSpriteCircleDef::Shape() const
	{
		return mShape;
	}

	Box2DSpritePolygonDef::Box2DSpritePolygonDef(const b2BodyDef& bodyDef, const b2PolygonShape& shape, const float friction, const float restitution, const float density, const bool isSensor) :
		Box2DSpriteDef(bodyDef, friction, restitution, density, isSensor),
		mShape(shape)
	{
	}

	const b2PolygonShape& Box2DSpritePolygonDef::Shape() const
	{
		return mShape;
	}

#pragma endregion

	RTTI_DEFINITIONS(Box2DSprite)

	Box2DSprite::Box2DSprite(Game& game, const shared_ptr<Camera>& camera, const shared_ptr<Texture2D>& texture, const Box2DSpriteDef& spriteDef, const XMFLOAT2& scale) :
		Sprite(game, camera, texture)
	{
		CreateBody(spriteDef);
		mBody->SetUserData(this);
		mTransform.SetScale(scale);
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

		if (mBody->GetType() != b2_staticBody && mBody->IsAwake())
		{
			UpdateTransformFromBody();
		}
	}

	shared_ptr<Box2DSprite> Box2DSprite::CreateBox(Game& game, const shared_ptr<Camera>& camera, const shared_ptr<Texture2D>& texture, FXMVECTOR position, const XMFLOAT2& scale)
	{
		return CreateBox(game, camera, texture, XMFLOAT2(XMVectorGetX(position), XMVectorGetY(position)), scale);
	}

	shared_ptr<Box2DSprite> Box2DSprite::CreateBox(Game& game, const shared_ptr<Camera>& camera, const shared_ptr<Texture2D>& texture, const XMFLOAT2& position, const XMFLOAT2& scale)
	{
		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		bodyDef.position.Set(position.x, position.y);

		b2PolygonShape shape;
		shape.SetAsBox(scale.x, scale.y);

		return make_shared<Box2DSprite>(game, camera, texture, Box2DSpritePolygonDef(bodyDef, shape), scale);
	}

	shared_ptr<Box2DSprite> Box2DSprite::CreateCircle(Game& game, const shared_ptr<Camera>& camera, const shared_ptr<Texture2D>& texture, FXMVECTOR position, const float radius)
	{
		return CreateCircle(game, camera, texture, XMFLOAT2(XMVectorGetX(position), XMVectorGetY(position)), radius);
	}
	
	shared_ptr<Box2DSprite> Box2DSprite::CreateCircle(Game& game, const shared_ptr<Camera>& camera, const shared_ptr<Texture2D>& texture, const XMFLOAT2& position, const float radius)
	{
		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		bodyDef.position.Set(position.x, position.y);

		b2CircleShape shape;
		shape.m_radius = radius;

		return make_shared<Box2DSprite>(game, camera, texture, Box2DSpriteCircleDef(bodyDef, shape), XMFLOAT2(radius, radius));
	}

	shared_ptr<Box2DSprite> Box2DSprite::CreateTriangle(Game& game, const shared_ptr<Camera>& camera, const shared_ptr<Texture2D>& texture, FXMVECTOR position, const XMFLOAT2& scale)
	{
		return CreateTriangle(game, camera, texture, XMFLOAT2(XMVectorGetX(position), XMVectorGetY(position)), scale);
	}

	shared_ptr<Box2DSprite> Box2DSprite::CreateTriangle(Game& game, const shared_ptr<Camera>& camera, const shared_ptr<Texture2D>& texture, const XMFLOAT2& position, const XMFLOAT2& scale)
	{
		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		bodyDef.position.Set(position.x, position.y);

		const b2Vec2 vertices[] =
		{
			{ scale.x, -scale.y },
			{ 0.0f, scale.y },
			{ -scale.x, -scale.y },
		};

		b2PolygonShape shape;
		shape.Set(vertices, ARRAYSIZE(vertices));		

		return make_shared<Box2DSprite>(game, camera, texture, Box2DSpritePolygonDef(bodyDef, shape), scale);
	}

	void Box2DSprite::CreateBody(const Box2DSpriteDef& spriteDef)
	{
		auto physicsEngine = reinterpret_cast<Box2DComponent*>(mGame->Services().GetService(Box2DComponent::TypeIdClass()));
		assert(physicsEngine != nullptr);

		b2World& world = physicsEngine->World();
		mBody = world.CreateBody(&spriteDef.BodyDef);

		b2FixtureDef fixtureDef;
		fixtureDef.shape = &(spriteDef.Shape());
		fixtureDef.friction = spriteDef.Friction;
		fixtureDef.restitution = spriteDef.Restitution;
		fixtureDef.density = spriteDef.Density;
		fixtureDef.filter = spriteDef.Filter;
		mBody->CreateFixture(&fixtureDef);
	}

	void Box2DSprite::UpdateTransformFromBody()
	{
		const auto& position = mBody->GetPosition();
		mTransform.SetPosition(position.x, position.y);
		mTransform.SetRotation(mBody->GetAngle());
	}
}

