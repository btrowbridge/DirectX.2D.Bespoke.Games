#include "pch.h"
#include "Bouncy.h"

using namespace Library;

namespace Reflection2DGame {
	Bouncy::Bouncy(Library::Game & game, const std::shared_ptr<Library::Camera>& camera, std::shared_ptr<Library::Box2DSprite>& sprite)
		: Box2DBehavior(game,camera,sprite,BehaviorType::Bouncy)
	{
	}
	void Bouncy::Initialize()
	{
		Box2DBehavior::Initialize();
		mKeyboard = static_cast<KeyboardComponent*>(mGame->Services().GetService(KeyboardComponent::TypeIdClass()));
	}
	void Bouncy::Update(const Library::GameTime & gameTime)
	{

		Box2DBehavior::Update(gameTime);

		if (mSprite->Body()->GetPosition().Length() > 20.0f || mKeyboard->WasKeyPressedThisFrame(Keys::R)) {
			mSprite->Body()->SetTransform(b2Vec2(0.0f, 5.0f),0.0f);
			mSprite->Body()->SetLinearVelocity(b2Vec2(0.0f, -5.0f));
		}
	}
	void Bouncy::Draw(const Library::GameTime & gameTime)
	{
		Box2DBehavior::Draw(gameTime);
	}

	void Bouncy::OnContactBegin(Box2DBehavior * other, b2Contact * contact)
	{
		UNREFERENCED_PARAMETER(other);
		UNREFERENCED_PARAMETER(contact);

		b2Vec2 direction = mSprite->Body()->GetLinearVelocity();
		

		int32 childIndex;
		b2Fixture* otherFixture;
		if (contact->GetFixtureA()->GetBody() == mSprite->Body()) {
			otherFixture = contact->GetFixtureB();
			childIndex = contact->GetChildIndexB();
		}
		else {
			otherFixture = contact->GetFixtureA();
			childIndex = contact->GetChildIndexA();
		}

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
