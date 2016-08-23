#include "pch.h"

using namespace std;
using namespace DirectX;

namespace Library
{
	RTTI_DEFINITIONS(AudioEngineComponent)

	AudioEngineComponent::AudioEngineComponent(Game& game) :
		GameComponent(game)
	{
		mAudioEngine = make_shared<DirectX::AudioEngine>(AudioEngine_Default);
	}

	bool AudioEngineComponent::LastUpdateResult() const
	{
		return mLastUpdateResult;
	}

	std::shared_ptr<DirectX::AudioEngine> AudioEngineComponent::AudioEngine() const
	{
		return mAudioEngine;
	}

	void AudioEngineComponent::Update(const GameTime& gameTime)
	{
		UNREFERENCED_PARAMETER(gameTime);

		mLastUpdateResult = mAudioEngine->Update();
	}
}