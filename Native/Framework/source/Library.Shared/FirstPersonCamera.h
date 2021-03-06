#pragma once

#include "PerspectiveCamera.h"
#include "GamePadComponent.h"
#include "FirstPersonController.h"

namespace Library
{	
	class KeyboardComponent;
	class MouseComponent;

    class FirstPersonCamera : public PerspectiveCamera
    {
		RTTI_DECLARATIONS(FirstPersonCamera, PerspectiveCamera)

    public:
		explicit FirstPersonCamera(Game& game, float fieldOfView = DefaultFieldOfView, float aspectRatio = DefaultAspectRatio, float nearPlaneDistance = DefaultNearPlaneDistance, float farPlaneDistance = DefaultFarPlaneDistance, float rotationRate = DefaultRotationRate, float movementRate = DefaultMovementRate);
		FirstPersonCamera(const FirstPersonCamera&) = delete;
		FirstPersonCamera& operator=(const FirstPersonCamera&) = delete;
		FirstPersonCamera(FirstPersonCamera&&) = delete;
		FirstPersonCamera& operator=(FirstPersonCamera&&) = delete;
		virtual ~FirstPersonCamera() = default;

		FirstPersonController& Controller();
		float& RotationRate();
		float& MovementRate();
        
		virtual void Initialize() override;
        virtual void Update(const GameTime& gameTime) override;

		static const float DefaultRotationRate;
		static const float DefaultMovementRate;

	private:
		void UpdatePosition(const DirectX::XMFLOAT2& movementAmount, const DirectX::XMFLOAT2& rotationAmount, const GameTime& gameTime);

    protected:
		FirstPersonController mController;
		float mRotationRate;
		float mMovementRate;
    };
}