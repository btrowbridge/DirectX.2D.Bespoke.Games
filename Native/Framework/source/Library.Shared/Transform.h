#pragma once

#include "RTTI.h"
#include "Quaternion.h"
#include <DirectXMath.h>

namespace Library
{
	class Transform final : public RTTI
	{
		RTTI_DECLARATIONS(Transform, RTTI)

	public:
		const DirectX::XMFLOAT3& Position() const;
		void SetPosition(const DirectX::XMFLOAT3& position);

		const Quaternion& Rotation() const;
		void SetRotation(const Quaternion& rotation);

		const DirectX::XMFLOAT3& Scale() const;
		void SetScale(const DirectX::XMFLOAT3& scale);

	private:
		DirectX::XMFLOAT3 mPosition;
		Quaternion mRotation;
		DirectX::XMFLOAT3 mScale;
	};
}

#include "Transform.inl"