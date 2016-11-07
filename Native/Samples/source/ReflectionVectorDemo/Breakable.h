#pragma once
#include "Box2DBehavior.h"




namespace Reflection2DGame {

	class Breakable : public Box2DBehavior
	{

	public:
		Breakable(Library::Game& game, const std::shared_ptr<Library::Camera>& camera, std::shared_ptr<Library::Box2DSprite>& sprite, float startHealth, int scorevalue);

		virtual void Initialize();
		virtual void Update(const Library::GameTime& gameTime);
		virtual void Draw(const Library::GameTime& gameTime);

		// Inherited via Box2DBehavior
		virtual void OnContactBegin(Box2DBehavior * other, b2Contact * contact) override;
		virtual void OnContactEnd(Box2DBehavior * other, b2Contact * contact) override;
		virtual void OnContactPreSolve(Box2DBehavior * other, b2Contact * contact, const b2Manifold * oldManifold) override;
		virtual void OnContactPostSolve(Box2DBehavior * other, b2Contact * contact, const b2ContactImpulse * impulse) override;

		
		int ScoreValue();
		void TakeDamage(float damageAmount);
		
	private:
		float mHealth;
		static const float mDamageThreshold;

		std::shared_ptr<DirectX::SpriteFont> mHealthFont;

		int mScoreValue;
	};

}