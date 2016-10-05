#include "pch.h"
#include "FirstPersonController.h"

using namespace DirectX;

namespace Library
{
	const float FirstPersonController::DefaultMouseSensitivity = 0.1f;

	FirstPersonController::FirstPersonController(Game& game) :
		FirstPersonController(game, nullptr)
	{
	}

	FirstPersonController::FirstPersonController(Game& game, UpdatePositionCallback callback) :
		GameComponent(game),
		mUpdatePositionCallback(callback),
		mGamePad(nullptr), mKeyboard(nullptr), mMouse(nullptr), mMouseSensitivity(DefaultMouseSensitivity)
	{
	}

	GamePadComponent* FirstPersonController::GetGamePad() const
	{
		return mGamePad;
	}

	void FirstPersonController::SetGamePad(GamePadComponent* gamePad)
	{
		mGamePad = gamePad;
	}

	KeyboardComponent* FirstPersonController::GetKeyboard() const
	{
		return mKeyboard;
	}

	void FirstPersonController::SetKeyboard(KeyboardComponent* keyboard)
	{
		mKeyboard = keyboard;
	}

	MouseComponent* FirstPersonController::GetMouse() const
	{
		return mMouse;
	}

	void FirstPersonController::SetMouse(MouseComponent* mouse)
	{
		mMouse = mouse;
	}

	float& FirstPersonController::MouseSensitivity()
	{
		return mMouseSensitivity;
	}

	FirstPersonController::UpdatePositionCallback FirstPersonController::GetUpdatePositionCallback() const
	{
		return mUpdatePositionCallback;
	}

	void FirstPersonController::SetUpdatePositionCallback(UpdatePositionCallback callback)
	{
		mUpdatePositionCallback = callback;
	}

	void FirstPersonController::Initialize()
	{
		mGamePad = reinterpret_cast<GamePadComponent*>(mGame->Services().GetService(GamePadComponent::TypeIdClass()));
		mKeyboard = reinterpret_cast<KeyboardComponent*>(mGame->Services().GetService(KeyboardComponent::TypeIdClass()));
		mMouse = reinterpret_cast<MouseComponent*>(mGame->Services().GetService(MouseComponent::TypeIdClass()));
	}

	void FirstPersonController::Update(const GameTime& gameTime)
	{
		GamePad::State gamePadState;
		if (IsGamePadConnected(gamePadState))
		{
			XMFLOAT2 movementAmount(gamePadState.thumbSticks.leftX, gamePadState.thumbSticks.leftY);
			XMFLOAT2 rotationAmount(-gamePadState.thumbSticks.rightX, gamePadState.thumbSticks.rightY);
			mUpdatePositionCallback( movementAmount, rotationAmount, gameTime);
		}
		else
		{
			bool positionChanged = false;
			XMFLOAT2 movementAmount = Vector2Helper::Zero;
			if (mKeyboard != nullptr)
			{
				if (mKeyboard->IsKeyDown(Keys::W))
				{
					movementAmount.y = 1.0f;
					positionChanged = true;
				}
				if (mKeyboard->IsKeyDown(Keys::S))
				{
					movementAmount.y = -1.0f;
					positionChanged = true;
				}
				if (mKeyboard->IsKeyDown(Keys::A))
				{
					movementAmount.x = -1.0f;
					positionChanged = true;
				}
				if (mKeyboard->IsKeyDown(Keys::D))
				{
					movementAmount.x = 1.0f;
					positionChanged = true;
				}
			}

			XMFLOAT2 rotationAmount = Vector2Helper::Zero;
			if (mMouse != nullptr)
			{
				if (mMouse->IsButtonHeldDown(MouseButtons::Left))
				{
					rotationAmount.x = -mMouse->X() * mMouseSensitivity;
					rotationAmount.y = -mMouse->Y() * mMouseSensitivity;
					positionChanged = true;
				}
			}

			if (positionChanged)
			{
				mUpdatePositionCallback(movementAmount, rotationAmount, gameTime);
			}
		}
	}
}