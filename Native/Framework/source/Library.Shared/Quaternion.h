#pragma once

#include "RTTI.h"
#include <DirectXMath.h>

namespace Library
{
	class Quaternion final : public RTTI
	{
		RTTI_DECLARATIONS(Quaternion, RTTI)

	public:
		static const Quaternion Identity;

		float X;
		float Y;
		float Z;
		float W;

		Quaternion();
		explicit Quaternion(DirectX::FXMVECTOR value);
		explicit Quaternion(const DirectX::XMFLOAT4& value);
		Quaternion(const DirectX::XMFLOAT3& value, float w);
		Quaternion(float x, float y, float z, float w);
		Quaternion(const Quaternion&) = default;
		Quaternion& operator=(const Quaternion&) = default;
		Quaternion(Quaternion&&) = default;
		Quaternion& operator=(Quaternion&&) = default;
		~Quaternion() = default;

		Quaternion operator+(const Quaternion& other);

		static Quaternion Add(const Quaternion& lhs, const Quaternion& rhss);
		static Quaternion Subtract(const Quaternion& lhs, const Quaternion& rhs);

		virtual std::string ToString() const override;
	};
}
