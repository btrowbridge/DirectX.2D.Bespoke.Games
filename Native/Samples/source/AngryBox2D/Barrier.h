#pragma once
#include "Box2DBehavior.h"

namespace AngryBox2DGame {

	class Barrier :
		public Box2DBehavior
	{
	public:
		Barrier(Library::Game& game, const std::shared_ptr<Library::Camera>& camera, std::shared_ptr<Library::Box2DSprite>& sprite);
	

		// Inherited via Box2DBehavior
		virtual void OnContactBegin(Box2DBehavior * other, b2Contact * contact) override;
		virtual void OnContactEnd(Box2DBehavior * other, b2Contact * contact) override;
		virtual void OnContactPreSolve(Box2DBehavior * other, b2Contact * contact, const b2Manifold * oldManifold) override;
		virtual void OnContactPostSolve(Box2DBehavior * other, b2Contact * contact, const b2ContactImpulse * impulse) override;
	};

}