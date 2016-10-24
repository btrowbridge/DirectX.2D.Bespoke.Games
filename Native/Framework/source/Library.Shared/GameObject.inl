#pragma once

namespace Library
{
	inline const Transform& GameObject::GetTransform() const
	{
		return mTransform;
	}

	inline void GameObject::SetTransform(const Transform& transform)
	{
		mTransform = transform;
	}
}