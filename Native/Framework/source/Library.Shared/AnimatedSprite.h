#pragma once

#include "DrawableGameComponent.h"

namespace Library
{
	class AnimatedSprite final : public DrawableGameComponent
	{
		RTTI_DECLARATIONS(AnimatedSprite, DrawableGameComponent)

	public:
		AnimatedSprite(Game& game, const std::shared_ptr<Camera>& camera);
		AnimatedSprite(const AnimatedSprite&) = default;
		AnimatedSprite& operator=(const AnimatedSprite&) = default;
		AnimatedSprite(AnimatedSprite&&) = default;
		AnimatedSprite& operator=(AnimatedSprite&&) = default;
		~AnimatedSprite() = default;

	private:
	};
}