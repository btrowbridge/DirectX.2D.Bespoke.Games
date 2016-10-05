#include "pch.h"

using namespace DirectX;

namespace Library
{
    RTTI_DEFINITIONS(FirstPersonCamera)

	const float FirstPersonCamera::DefaultRotationRate = XMConvertToRadians(100.0f);
	const float FirstPersonCamera::DefaultMovementRate = 100.0f;

	FirstPersonCamera::FirstPersonCamera(Game& game, float fieldOfView, float aspectRatio, float nearPlaneDistance, float farPlaneDistance, float rotationRate, float movementRate) :
		PerspectiveCamera(game, fieldOfView, aspectRatio, nearPlaneDistance, farPlaneDistance),
		mController(game), mRotationRate(rotationRate), mMovementRate(movementRate)
	{
	}

	FirstPersonController& FirstPersonCamera::Controller()
	{
		return mController;
	}

	float& FirstPersonCamera::RotationRate()
	{
		return mRotationRate;
	}

	float& FirstPersonCamera::MovementRate()
	{
		return mMovementRate;
	}

	void FirstPersonCamera::Initialize()
	{
		using namespace std::placeholders;
		auto callback = std::bind(&FirstPersonCamera::UpdatePosition, this, _1, _2, _3);
		mController.SetUpdatePositionCallback(callback);
		mController.Initialize();

		Camera::Initialize();
	}

    void FirstPersonCamera::Update(const GameTime& gameTime)
    {		
		mController.Update(gameTime);
        Camera::Update(gameTime);
    }

	void FirstPersonCamera::UpdatePosition(const XMFLOAT2& movementAmount, const XMFLOAT2& rotationAmount, const GameTime& gameTime)
	{
		float elapsedTime = gameTime.ElapsedGameTimeSeconds().count();
		XMVECTOR rotationVector = XMLoadFloat2(&rotationAmount) * mRotationRate * elapsedTime;
		XMVECTOR right = XMLoadFloat3(&mRight);

		XMMATRIX pitchMatrix = XMMatrixRotationAxis(right, XMVectorGetY(rotationVector));
		XMMATRIX yawMatrix = XMMatrixRotationY(XMVectorGetX(rotationVector));

		ApplyRotation(XMMatrixMultiply(pitchMatrix, yawMatrix));

		XMVECTOR position = XMLoadFloat3(&mPosition);
		XMVECTOR movement = XMLoadFloat2(&movementAmount) * mMovementRate * elapsedTime;

		XMVECTOR strafe = right * XMVectorGetX(movement);
		position += strafe;

		XMVECTOR forward = XMLoadFloat3(&mDirection) * XMVectorGetY(movement);
		position += forward;

		XMStoreFloat3(&mPosition, position);
	}
}
