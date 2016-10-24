#pragma once

#include "OrthographicCamera.h"

namespace Library
{
	class FirstPersonOrthographicCamera final : public OrthographicCamera
	{
	public:
		FirstPersonOrthographicCamera(Game& game, float viewWidth = DefaultViewWidth, float viewHeight = DefaultViewHeight, float nearPlaneDistance = DefaultNearPlaneDistance, float farPlaneDistance = DefaultFarPlaneDistance);
		OrthographicCamera(const OrthographicCamera&) = delete;
		OrthographicCamera(OrthographicCamera&&) = delete;
		OrthographicCamera& operator=(const OrthographicCamera&) = delete;
		OrthographicCamera& operator=(OrthographicCamera&&) = default;
		~FirstPersonOrthographicCamera() = default;
	};
}