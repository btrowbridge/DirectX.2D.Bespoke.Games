#pragma once

#include "GameComponent.h"
#include <Audio.h>
#include <memory>

namespace Library
{
	class AudioEngineComponent final : public GameComponent
	{
		RTTI_DECLARATIONS(GamePadComponent, GameComponent)

	public:
		AudioEngineComponent(Game& game);

		bool LastUpdateResult() const;
		std::shared_ptr<DirectX::AudioEngine> AudioEngine() const;

		virtual void Update(const GameTime& gameTime) override;

	private:
		std::shared_ptr<DirectX::AudioEngine> mAudioEngine;
		bool mLastUpdateResult;
	};
}