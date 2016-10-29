#include "pch.h"
#include "Barrier.h"

using namespace Library;
namespace AngryBox2DGame {

	AngryBox2DGame::Barrier::Barrier(Library::Game & game, const std::shared_ptr<Library::Camera>& camera, std::shared_ptr<Library::Box2DSprite>& sprite) 
		: Box2DBehavior(game, camera, sprite, BehaviorType::Barrier)
	{
		
	}

	void Barrier::OnContactBegin(Box2DBehavior * other, b2Contact * contact)
	{		
		UNREFERENCED_PARAMETER(other);
		UNREFERENCED_PARAMETER(contact);
	}

	void Barrier::OnContactEnd(Box2DBehavior * other, b2Contact * contact)
	{	
		auto physicsEngine = reinterpret_cast<Box2DComponent*>(mGame->Services().GetService(Box2DComponent::TypeIdClass()));

		if (other == nullptr) {
			b2Body* otherBody = (contact->GetFixtureA()->IsSensor()) ? contact->GetFixtureB()->GetBody() : contact->GetFixtureA()->GetBody();
			physicsEngine->BuryBody(*otherBody);
		}

		switch (other->Tag()) {
		case BehaviorType::Ammunition:
			other->As<Ammunition>()->Reset();
		default:
			other->ScheduleToDestroy();
		}

	}

	void Barrier::OnContactPreSolve(Box2DBehavior * other, b2Contact * contact, const b2Manifold * oldManifold)
	{
		UNREFERENCED_PARAMETER(other);
		UNREFERENCED_PARAMETER(contact);
		UNREFERENCED_PARAMETER(oldManifold);
	}

	void Barrier::OnContactPostSolve(Box2DBehavior * other, b2Contact * contact, const b2ContactImpulse * impulse)
	{	
		UNREFERENCED_PARAMETER(other);
		UNREFERENCED_PARAMETER(contact);
		UNREFERENCED_PARAMETER(impulse);
	}
}