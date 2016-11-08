#include "pch.h"
#include "Bouncy.h"

namespace Reflection2DGame {
	Bouncy::Bouncy(Library::Game & game, const std::shared_ptr<Library::Camera>& camera, std::shared_ptr<Library::Box2DSprite>& sprite)
		: Box2DBehavior(game,camera,sprite,BehaviorType::Bouncy)
	{
	}
	void Bouncy::Initialize()
	{
	}
	void Bouncy::Update(const Library::GameTime & gameTime)
	{
		UNREFERENCED_PARAMETER(gameTime);

		if (mSprite->Body()->GetPosition().Length() > 50.0f) {
			this->ScheduleToDestroy();
		}
	}
	void Bouncy::Draw(const Library::GameTime & gameTime)
	{
		UNREFERENCED_PARAMETER(gameTime);
	}

	void Bouncy::OnContactBegin(Box2DBehavior * other, b2Contact * contact)
	{
		UNREFERENCED_PARAMETER(other);
		UNREFERENCED_PARAMETER(contact);

		b2Vec2 direction = mSprite->Body()->GetLinearVelocity();
		b2WorldManifold worldManifold;

		int32 childIndex;
		b2Fixture* otherFixture;
		if (contact->GetFixtureA()->GetBody() == mSprite->Body()) {
			otherFixture = contact->GetFixtureB();
			childIndex = contact->GetChildIndexB();
		}
		else {
			otherFixture = contact->GetFixtureA();
			childIndex = contact->GetChildIndexB();
		}

		b2Vec2 unitDir = direction;
		unitDir.Normalize();

		b2RayCastInput rayCastIN;
		rayCastIN.p1 = mSprite->Body()->GetPosition();
		rayCastIN.p2 = direction;
		rayCastIN.maxFraction = 1;

		b2RayCastOutput rayCastOUT;


		otherFixture->RayCast(&rayCastOUT, rayCastIN, childIndex);

		b2Vec2 normal = rayCastOUT.normal;
		normal.Normalize();
		
		
		b2Vec2 reflection = direction - 2 * b2Dot(direction, normal) * normal; //Reflection math
		
		mSprite->Body()->SetLinearVelocity(reflection);
	}

	void Bouncy::OnContactEnd(Box2DBehavior * other, b2Contact * contact)
	{	
		UNREFERENCED_PARAMETER(other);
		UNREFERENCED_PARAMETER(contact);

	}
	void Bouncy::OnContactPreSolve(Box2DBehavior * other, b2Contact * contact, const b2Manifold * oldManifold)
	{
		UNREFERENCED_PARAMETER(other);
		UNREFERENCED_PARAMETER(contact);
		UNREFERENCED_PARAMETER(oldManifold);

	}
	void Bouncy::OnContactPostSolve(Box2DBehavior * other, b2Contact * contact, const b2ContactImpulse * impulse)
	{
		UNREFERENCED_PARAMETER(other);
		UNREFERENCED_PARAMETER(contact);
		UNREFERENCED_PARAMETER(impulse);
	}
}
