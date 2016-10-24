#include "pch.h"
#include "Transform2D.h"

using namespace DirectX;

namespace Library
{
	RTTI_DEFINITIONS(Transform2D)

	Transform2D::Transform2D(const XMFLOAT2& position, float rotation, const XMFLOAT2& scale) :
		mPosition(position), mRotation(rotation), mScale(scale)
	{
	}
}