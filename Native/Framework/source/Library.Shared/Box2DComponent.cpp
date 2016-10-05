#include "pch.h"

using namespace std;
using namespace DirectX;

namespace Library
{
	RTTI_DEFINITIONS(Box2DComponent)

	const b2Vec2 Box2DComponent::DefaultGravity = { 0.0f, -10.0f };
	const float Box2DComponent::DefaultTimeStep = 1.0f / 60.0f;
	const int32_t Box2DComponent::DefaultVelocityIterations = 8;
	const int32_t Box2DComponent::DefaultPositionIterations = 3;

	Box2DComponent::Box2DComponent(Game& game, const b2Vec2& gravity, float timeStep, std::int32_t velocityIterations, std::int32_t positionIterations) :
		GameComponent(game), mGravity(gravity), mWorld(make_unique<b2World>(gravity)), mTimeStep(timeStep),
		mVelocityIterations(velocityIterations), mPositionIterations(positionIterations)
	{
	}

	const b2Vec2& Box2DComponent::Gravity() const
	{
		return mGravity;
	}

	b2World& Box2DComponent::World()
	{
		return *mWorld;
	}

	float Box2DComponent::TimeStep() const
	{
		return mTimeStep;
	}

	void Box2DComponent::SetTimeStep(float timeStep)
	{
		mTimeStep = timeStep;
	}

	std::int32_t Box2DComponent::VelocityIterations() const
	{
		return mVelocityIterations;
	}

	void Box2DComponent::SetVelocityIterations(std::int32_t velocityIterations)
	{
		mVelocityIterations = velocityIterations;
	}

	std::int32_t Box2DComponent::PositionIterations() const
	{
		return mPositionIterations;
	}

	void Box2DComponent::SetPositionIterations(std::int32_t positionIterations)
	{
		mPositionIterations = positionIterations;
	}

	void Box2DComponent::Clear()
	{
		mWorld = make_unique<b2World>(mGravity);
	}
	
	void Box2DComponent::Update(const GameTime& gameTime)
	{
		UNREFERENCED_PARAMETER(gameTime);

		mWorld->Step(mTimeStep, mVelocityIterations, mPositionIterations);
	}
}