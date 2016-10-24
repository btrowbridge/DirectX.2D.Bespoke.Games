#pragma once

#include "DrawableGameComponent.h"
#include "Transform.h"

namespace Library
{
	class GameObject : public DrawableGameComponent
	{
		RTTI_DECLARATIONS(GameObject, DrawableGameComponent)

	public:
		GameObject() = default;
		GameObject(Game& game);
		GameObject(Game& game, const std::shared_ptr<Camera>& camera);
		GameObject(const GameObject&) = default;
		GameObject& operator=(const GameObject&) = default;
		GameObject(GameObject&&) = default;
		GameObject& operator=(GameObject&&) = default;
		virtual ~GameObject() = default;

		const Transform& GetTransform() const;
		void SetTransform(const Transform& transform);

	private:
		Transform mTransform;
	};
}

#include "GameObject.inl"