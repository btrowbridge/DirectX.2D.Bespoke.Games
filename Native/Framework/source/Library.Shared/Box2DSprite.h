#pragma once

#include "Sprite.h"
#include "VectorHelper.h"
#include <Box2D\Box2D.h>

namespace Library
{
	class Box2DSprite final : public Sprite
	{
		RTTI_DECLARATIONS(Box2DSprite, Sprite)

	public:
		//TODO add size
		//TODO: perhaps include a 2D transform constructor
		Box2DSprite(Game& game, const std::shared_ptr<Camera>& camera, const std::shared_ptr<Texture2D>& texture, const DirectX::XMFLOAT2& position = Vector2Helper::Zero);
		Box2DSprite(Game& game, const std::shared_ptr<Camera>& camera, const std::shared_ptr<Texture2D>& texture, const b2BodyDef& bodyDef);
		Box2DSprite(const Box2DSprite&) = delete;
		Box2DSprite& operator=(const Box2DSprite&) = delete;
		Box2DSprite(Box2DSprite&&) = default;
		Box2DSprite& operator=(Box2DSprite&&) = default;
		~Box2DSprite();

		b2Body* Body() const;

		virtual void Initialize() override;
		virtual void Update(const GameTime& gameTime) override;

	private:
		void CreateBody(const b2BodyDef& bodyDef);
		void UpdateTransformFromBody();

		b2Body* mBody;
	};
}