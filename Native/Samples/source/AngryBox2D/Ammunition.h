#pragma once
#include "Box2DBehavior.h"

namespace AngryBox2DGame {

	class Ammunition : public Box2DBehavior
	{
	public:
		Ammunition(Library::Game& game, const std::shared_ptr<Library::Camera>& camera, std::shared_ptr<Library::Box2DSprite>& sprite, b2Body* base, b2Vec2 target);

		virtual void Initialize();
		virtual void Update(const Library::GameTime& gameTime);
		virtual void Draw(const Library::GameTime& gameTime);

		// Inherited via Box2DBehavior
		virtual void OnContactBegin(Box2DBehavior * other, b2Contact * contact) override;
		virtual void OnContactEnd(Box2DBehavior * other, b2Contact * contact) override;
		virtual void OnContactPreSolve(Box2DBehavior * other, b2Contact * contact, const b2Manifold * oldManifold) override;
		virtual void OnContactPostSolve(Box2DBehavior * other, b2Contact * contact, const b2ContactImpulse * impulse) override;
		
		void AddForceToTarget(b2Vec2 direction);
		void Reset();
		void Launch();

		void OnClick();
		bool Ready();
	private:


		b2MouseJointDef mJointDef;

		b2Body* mBase;

		b2Vec2 mTarget;

		b2Joint* mSlingJoint;

		bool mWasClicked;
		bool mReadyToFire;
		bool mWasFired;
	};

}