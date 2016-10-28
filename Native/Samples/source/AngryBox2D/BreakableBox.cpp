#include "pch.h"
#include "BreakableBox.h"
#include <numeric>

namespace AngryBox2DGame {
	BreakableBox::BreakableBox(Library::Game & game, const std::shared_ptr<Library::Camera>& camera, Library::Box2DSprite * sprite, float32 startHealth) :
		Box2DBehavior(game,camera,sprite,"Breakable"), mHealth(startHealth)
	{

	}
	void BreakableBox::Initialize()
	{
		Box2DBehavior::Initialize();
	}
	void BreakableBox::Update(const Library::GameTime & gameTime)
	{
		Box2DBehavior::Update(gameTime);
	}
	void BreakableBox::Draw(const Library::GameTime & gameTime)
	{
		Box2DBehavior::Draw(gameTime);
	}

	void BreakableBox::OnContactBegin(Box2DBehavior * other, b2Contact * contact)
	{		
		UNREFERENCED_PARAMETER(other);
		UNREFERENCED_PARAMETER(contact);
	}

	void BreakableBox::OnContactEnd(Box2DBehavior * other, b2Contact * contact)
	{
		UNREFERENCED_PARAMETER(other);
		UNREFERENCED_PARAMETER(contact);

	}

	void BreakableBox::OnContactPreSolve(Box2DBehavior * other, b2Contact * contact, const b2Manifold * oldManifold)
	{
		UNREFERENCED_PARAMETER(other);
		UNREFERENCED_PARAMETER(contact);
		UNREFERENCED_PARAMETER(oldManifold);
	}

	void BreakableBox::OnContactPostSolve(Box2DBehavior * other, b2Contact * contact, const b2ContactImpulse * impulse)
	{	
		UNREFERENCED_PARAMETER(other);
		UNREFERENCED_PARAMETER(contact);
		//Calculate damages from impulse forces
		float32 calculatedDamage = std::accumulate(std::begin(impulse->normalImpulses), std::end(impulse->normalImpulses), 0.0f);
		TakeDamage(calculatedDamage);
	}
	void BreakableBox::Destroy()
	{
		Box2DBehavior::Destroy();
	}
	void BreakableBox::TakeDamage(float32 damageAmount)
	{
		mHealth = damageAmount;
		if (mHealth <= 0) {
			Destroy();
		}
	}
}