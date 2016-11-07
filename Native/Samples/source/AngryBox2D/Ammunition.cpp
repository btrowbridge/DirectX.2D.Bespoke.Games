#include "pch.h"
#include "Ammunition.h"

using namespace std;
using namespace Library;
using namespace DirectX;

namespace AngryBox2DGame {

	

	Ammunition::Ammunition(Library::Game & game, const std::shared_ptr<Library::Camera>& camera, std::shared_ptr<Library::Box2DSprite>& sprite, b2Body* ground, b2Vec2 target) :
		Box2DBehavior(game, camera, sprite, BehaviorType::Ammunition), mBase(ground), mSlingJoint(nullptr),
		mTarget(target), mWasClicked(false), mReadyToFire(false), mWasFired(false), mScheduledToResetLater(false)
	{
		
	}
	void Ammunition::Initialize()
	{
		Box2DBehavior::Initialize();
		Reset();
	}
	void Ammunition::Update(const Library::GameTime & gameTime)
	{
		Box2DBehavior::Update(gameTime);

		b2Body* body = mSprite->Body();
		b2Vec2 directionToSling = mTarget - mSprite->As<Box2DSprite>()->Body()->GetPosition();
		float distanceToSling = directionToSling.Length();
		if (mWasClicked) {
			auto mouse = reinterpret_cast<MouseComponent*>(mGame->Services().GetService(MouseComponent::TypeIdClass()));
			if (distanceToSling < 1.0f && mReadyToFire && !mouse->IsButtonDown(MouseButtons::Right) && mSlingJoint != nullptr) {
				Launch();
			} else if (distanceToSling > 2.0f) {
			mReadyToFire = true;
				if (!body->IsAwake()) {
					Reset();
				}
			} 
		} else {
			AddForceToTarget(directionToSling);
		}

		if (mScheduledToResetLater) {
			ScheduledReset();
		}


	}
	void Ammunition::Draw(const Library::GameTime & gameTime)
	{
		Box2DBehavior::Draw(gameTime);
	}

	void Ammunition::OnContactBegin(Box2DBehavior * other, b2Contact * contact)
	{		
		UNREFERENCED_PARAMETER(other);
		UNREFERENCED_PARAMETER(contact);
	}

	void Ammunition::OnContactEnd(Box2DBehavior * other, b2Contact * contact)
	{
		UNREFERENCED_PARAMETER(other);
		UNREFERENCED_PARAMETER(contact);

	}

	void Ammunition::OnContactPreSolve(Box2DBehavior * other, b2Contact * contact, const b2Manifold * oldManifold)
	{
		UNREFERENCED_PARAMETER(other);
		UNREFERENCED_PARAMETER(contact);
		UNREFERENCED_PARAMETER(oldManifold);
	}

	void Ammunition::OnContactPostSolve(Box2DBehavior * other, b2Contact * contact, const b2ContactImpulse * impulse)
	{	
		UNREFERENCED_PARAMETER(other);
		UNREFERENCED_PARAMETER(contact);
		UNREFERENCED_PARAMETER(impulse);
	}

	void Ammunition::AddForceToTarget(b2Vec2 direction)
	{
		static const float mag = 100.0f;
		direction.Set(direction.x * mag, direction.y * mag);
		if (mSlingJoint != nullptr) {
			auto body = mSprite->As<Box2DSprite>()->Body();
			
			body->ApplyLinearImpulseToCenter(direction, true);
		}
	}

	void Ammunition::Reset()
	{	
		mScheduledToResetLater = true;
	}

	void Ammunition::Launch()
	{
		auto physicsEngine = reinterpret_cast<Box2DComponent*>(mGame->Services().GetService(Box2DComponent::TypeIdClass()));
		physicsEngine->ScheduleJointForDestruction(*mSlingJoint);
		mSlingJoint = nullptr;

		mReadyToFire = false;
		mWasFired = true;
	}

	void AngryBox2DGame::Ammunition::OnClick()
	{
		mWasClicked = true;
	}
	bool Ammunition::Ready()
	{
		return (!mWasFired);
	}
	void Ammunition::ScheduledReset()
	{
		auto physicsEngine = reinterpret_cast<Box2DComponent*>(mGame->Services().GetService(Box2DComponent::TypeIdClass()));
		
		auto body = mSprite->Body();
		if (body->GetJointList() == nullptr) {

			body->SetTransform(mTarget, 0.0f);
			static const float forceMultiplier = 1000.0f;

			mJointDef.bodyA = mBase;
			mJointDef.bodyB = body;
			mJointDef.target = mTarget;
			mJointDef.maxForce = forceMultiplier * mSprite->As<Box2DSprite>()->Body()->GetMass();

			mSlingJoint = physicsEngine->World().CreateJoint(&mJointDef);


			mReadyToFire = false;
			mWasFired = false;
			mWasClicked = false;
			mScheduledToResetLater = false;
		}
	}
}