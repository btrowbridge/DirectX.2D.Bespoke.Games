#pragma once

#include "GamePadComponent.h"
#include <functional>

namespace Library
{
	class KeyboardComponent;
	class MouseComponent;

	class FirstPersonController final : public GameComponent
	{
	public:
		typedef std::function<void(const DirectX::XMFLOAT2&, const DirectX::XMFLOAT2&, const GameTime&)> UpdatePositionCallback;

		explicit FirstPersonController(Game& game);
		FirstPersonController(Game& game, UpdatePositionCallback callback);

		GamePadComponent* GetGamePad() const;
		void SetGamePad(GamePadComponent* gamePad);

		KeyboardComponent* GetKeyboard() const;
		void SetKeyboard(KeyboardComponent* keyboard);

		MouseComponent* GetMouse() const;
		void SetMouse(MouseComponent* mouse);

		float& MouseSensitivity();		

		UpdatePositionCallback GetUpdatePositionCallback() const;
		void SetUpdatePositionCallback(UpdatePositionCallback callback);

		virtual void Initialize() override;
		virtual void Update(const GameTime& gameTime) override;

		static const float DefaultMouseSensitivity;

	private:
		inline bool IsGamePadConnected(DirectX::GamePad::State& gamePadState)
		{
			if (mGamePad != nullptr)
			{
				gamePadState = mGamePad->CurrentState();
				return gamePadState.IsConnected();
			}

			return false;
		}

		GamePadComponent* mGamePad;
		KeyboardComponent* mKeyboard;
		MouseComponent* mMouse;
		float mMouseSensitivity;
		UpdatePositionCallback mUpdatePositionCallback;
	};
}