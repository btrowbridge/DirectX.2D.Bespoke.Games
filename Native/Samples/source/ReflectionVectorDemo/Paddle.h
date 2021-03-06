#pragma once
#include "Box2DBehavior.h"

namespace Library {
	class KeyboardComponent;
}

namespace Reflection2DGame {

	class Paddle :
		public Box2DBehavior
	{
	public:
		Paddle(Library::Game& game, const std::shared_ptr<Library::Camera>& camera, std::shared_ptr<Library::Box2DSprite>& sprite);
		
		virtual void Initialize()override;
		virtual void Update(const Library::GameTime& gameTime)override;
		virtual void Draw(const Library::GameTime& gameTime)override;

		// Inherited via Box2DBehavior
		virtual void OnContactBegin(Box2DBehavior * other, b2Contact * contact) override;

		virtual void OnContactEnd(Box2DBehavior * other, b2Contact * contact) override;

		virtual void OnContactPreSolve(Box2DBehavior * other, b2Contact * contact, const b2Manifold * oldManifold) override;

		virtual void OnContactPostSolve(Box2DBehavior * other, b2Contact * contact, const b2ContactImpulse * impulse) override;

	private:

		const static float32 mRotationSpeed;

		Library::KeyboardComponent* mKeyboard;



	};

}