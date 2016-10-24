#pragma once

namespace Library
{
	inline const DirectX::XMFLOAT3& Transform::Position() const
	{
		return mPosition;
	}

	inline void Transform::SetPosition(const DirectX::XMFLOAT3& position)
	{
		mPosition = position;
	}

	inline const Quaternion& Transform::Rotation() const
	{
		return mRotation;
	}

	inline void Transform::SetRotation(const Quaternion& rotation)
	{
		mRotation = rotation;
	}

	inline const DirectX::XMFLOAT3& Transform::Scale() const
	{
		return mScale;
	}

	inline void Transform::SetScale(const DirectX::XMFLOAT3& scale)
	{
		mScale = scale;
	}
}