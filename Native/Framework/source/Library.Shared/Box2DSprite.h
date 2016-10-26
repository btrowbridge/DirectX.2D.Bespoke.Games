#pragma once

#include "Sprite.h"
#include "VectorHelper.h"
#include <Box2D\Box2D.h>

namespace Library
{
	class Box2DSpriteDef abstract
	{
	public:
		static const float DefaultFriction;
		static const float DefaultRestitution;
		static const float DefaultDensity;

		Box2DSpriteDef() = default;
		Box2DSpriteDef(const b2BodyDef& bodyDef, const float friction = DefaultFriction, const float restitution = DefaultRestitution, const float density = DefaultDensity, const bool isSensor = false);
		Box2DSpriteDef(const Box2DSpriteDef&) = default;
		Box2DSpriteDef& operator=(const Box2DSpriteDef&) = default;
		Box2DSpriteDef(Box2DSpriteDef&&) = default;
		Box2DSpriteDef& operator=(Box2DSpriteDef&&) = default;
		virtual ~Box2DSpriteDef() = default;

		b2BodyDef BodyDef;
		float Friction;
		float Restitution;
		float Density;
		bool IsSensor;
		b2Filter Filter;

		virtual const b2Shape& Shape() const = 0;
	};

	class Box2DSpriteCircleDef final : public Box2DSpriteDef
	{
	public:
		Box2DSpriteCircleDef() = default;
		Box2DSpriteCircleDef(const b2BodyDef& bodyDef, const b2CircleShape& shape, const float friction = DefaultFriction, const float restitution = DefaultRestitution, const float density = DefaultDensity, const bool isSensor = false);
		Box2DSpriteCircleDef(const Box2DSpriteCircleDef&) = default;
		Box2DSpriteCircleDef& operator=(const Box2DSpriteCircleDef&) = default;
		Box2DSpriteCircleDef(Box2DSpriteCircleDef&&) = default;
		Box2DSpriteCircleDef& operator=(Box2DSpriteCircleDef&&) = default;
		~Box2DSpriteCircleDef() = default;

		virtual const b2CircleShape& Shape() const override;

	private:
		b2CircleShape mShape;
	};

	class Box2DSpritePolygonDef final : public Box2DSpriteDef
	{
	public:
		Box2DSpritePolygonDef() = default;
		Box2DSpritePolygonDef(const b2BodyDef& bodyDef, const b2PolygonShape& shape, const float friction = DefaultFriction, const float restitution = DefaultRestitution, const float density = DefaultDensity, const bool isSensor = false);
		Box2DSpritePolygonDef(const Box2DSpritePolygonDef&) = default;
		Box2DSpritePolygonDef& operator=(const Box2DSpritePolygonDef&) = default;
		Box2DSpritePolygonDef(Box2DSpritePolygonDef&&) = default;
		Box2DSpritePolygonDef& operator=(Box2DSpritePolygonDef&&) = default;
		~Box2DSpritePolygonDef() = default;

		virtual const b2PolygonShape& Shape() const override;

	private:
		b2PolygonShape mShape;
	};

	class Box2DSprite final : public Sprite
	{
		RTTI_DECLARATIONS(Box2DSprite, Sprite)

	public:
		Box2DSprite(Game& game, const std::shared_ptr<Camera>& camera, const std::shared_ptr<Texture2D>& texture, const Box2DSpriteDef& spriteDef, const DirectX::XMFLOAT2& scale = Vector2Helper::One);
		Box2DSprite(const Box2DSprite&) = delete;
		Box2DSprite& operator=(const Box2DSprite&) = delete;
		Box2DSprite(Box2DSprite&&) = default;
		Box2DSprite& operator=(Box2DSprite&&) = default;
		~Box2DSprite();

		b2Body* Body() const;

		virtual void Initialize() override;
		virtual void Update(const GameTime& gameTime) override;

		static std::shared_ptr<Box2DSprite> CreateBox(Game& game, const std::shared_ptr<Camera>& camera, const std::shared_ptr<Texture2D>& texture, DirectX::FXMVECTOR position, const DirectX::XMFLOAT2& scale = Vector2Helper::One);
		static std::shared_ptr<Box2DSprite> CreateBox(Game& game, const std::shared_ptr<Camera>& camera, const std::shared_ptr<Texture2D>& texture, const DirectX::XMFLOAT2& position, const DirectX::XMFLOAT2& scale = Vector2Helper::One);
		
		static std::shared_ptr<Box2DSprite> CreateCircle(Game& game, const std::shared_ptr<Camera>& camera, const std::shared_ptr<Texture2D>& texture, DirectX::FXMVECTOR position, const float radius);
		static std::shared_ptr<Box2DSprite> CreateCircle(Game& game, const std::shared_ptr<Camera>& camera, const std::shared_ptr<Texture2D>& texture, const DirectX::XMFLOAT2& position, const float radius);

		static std::shared_ptr<Box2DSprite> CreateTriangle(Game& game, const std::shared_ptr<Camera>& camera, const std::shared_ptr<Texture2D>& texture, DirectX::FXMVECTOR position, const DirectX::XMFLOAT2& scale = Vector2Helper::One);
		static std::shared_ptr<Box2DSprite> CreateTriangle(Game& game, const std::shared_ptr<Camera>& camera, const std::shared_ptr<Texture2D>& texture, const DirectX::XMFLOAT2& position, const DirectX::XMFLOAT2& scale = Vector2Helper::One);

	private:
		void CreateBody(const Box2DSpriteDef& spriteDef);
		void UpdateTransformFromBody();

		b2Body* mBody;
	};
}