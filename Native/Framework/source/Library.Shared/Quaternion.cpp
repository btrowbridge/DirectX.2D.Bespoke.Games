#include "pch.h"
#include "Quaternion.h"

using namespace std;
using namespace DirectX;

namespace Library
{
	RTTI_DEFINITIONS(Quaternion)

	const Quaternion Quaternion::Identity = { 0, 0, 0, 1 };

	Quaternion::Quaternion() :
		Quaternion(Identity.X, Identity.Y, Identity.Z, Identity.W)
	{
	}

	Quaternion::Quaternion(FXMVECTOR value) :
		Quaternion(XMVectorGetX(value), XMVectorGetY(value), XMVectorGetZ(value), XMVectorGetW(value))
	{
	}

	Quaternion::Quaternion(const DirectX::XMFLOAT4& value) :
		Quaternion(value.x, value.y, value.z, value.w)
	{
	}

	Quaternion::Quaternion(const DirectX::XMFLOAT3& value, float w) : 
		Quaternion(value.x, value.y, value.z, w)
	{
	}
	
	Quaternion::Quaternion(float x, float y, float z, float w) :
		X(x), Y(y), Z(z), W(w)
	{
	}

	Quaternion Quaternion::operator+(const Quaternion& other)
	{
		return Add(*this, other);
	}

	Quaternion Quaternion::Add(const Quaternion& lhs, const Quaternion& rhs)
	{
		return Quaternion(lhs.X + rhs.X, lhs.Y + rhs.Y, lhs.Z + rhs.Z, lhs.W + rhs.W);
	}

	Quaternion Quaternion::Subtract(const Quaternion& lhs, const Quaternion& rhs)
	{
		return Quaternion(lhs.X - rhs.X, lhs.Y - rhs.Y, lhs.Z - rhs.Z, lhs.W - rhs.W);
	}

	string Quaternion::ToString() const
	{
		ostringstream stream;
		stream << X << ", " << Y << ", " << Z << ", " << W;

		return stream.str();
	}
}
