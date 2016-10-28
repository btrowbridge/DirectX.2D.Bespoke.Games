#pragma once
#include "Box2DBehavior.h"

namespace AngryBox2DGame {

	class BreakableBox : public Box2DBehavior
	{
	public:
		BreakableBox(Library::Game& game, const std::shared_ptr<Library::Camera>& camera, Library::Box2DSprite* sprite, float32 startHealth);

		virtual void Initialize();
		virtual void Update(const Library::GameTime& gameTime);
		virtual void Draw(const Library::GameTime& gameTime);

		// Inherited via Box2DBehavior
		virtual void OnContactBegin(Box2DBehavior * other, b2Contact * contact) override;
		virtual void OnContactEnd(Box2DBehavior * other, b2Contact * contact) override;
		virtual void OnContactPreSolve(Box2DBehavior * other, b2Contact * contact, const b2Manifold * oldManifold) override;
		virtual void OnContactPostSolve(Box2DBehavior * other, b2Contact * contact, const b2ContactImpulse * impulse) override;

		virtual void Destroy() override;

		void TakeDamage(float32 damageAmount );
		
	private:
		float32 mHealth;

	};

}