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
	}
	void Bouncy::Draw(const Library::GameTime & gameTime)
	{
		UNREFERENCED_PARAMETER(gameTime);
	}
	//Manual reflection math
	void Bouncy::OnContactBegin(Box2DBehavior * other, b2Contact * contact)
	{
		UNREFERENCED_PARAMETER(other);
		b2Vec2 direction = mSprite->Body()->GetLinearVelocity();

		b2WorldManifold worldManifold;
		contact->GetWorldManifold(&worldManifold); 
		b2Vec2 worldNormal = worldManifold.normal;

		b2Vec2 reflection = direction - 2 * b2Dot(direction, worldNormal) * worldNormal;

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