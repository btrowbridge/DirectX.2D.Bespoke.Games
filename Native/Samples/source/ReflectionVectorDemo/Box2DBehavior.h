#pragma once
#include <string>
#include "DrawableGameComponent.h"

namespace Library {
	class Box2DSprite;
	class Camera;
}

namespace Reflection2DGame {
	enum class BehaviorType;

	class Box2DBehavior abstract : public Library::DrawableGameComponent
	{

	public:
		Box2DBehavior(Library::Game& game, const std::shared_ptr<Library::Camera>& camera, std::shared_ptr<Library::Box2DSprite>& sprite, BehaviorType tag );
		~Box2DBehavior();

		Library::Box2DSprite* Sprite() const;

		BehaviorType Tag();

		virtual void Initialize();
		virtual void Update(const Library::GameTime& gameTime);
		virtual void Draw(const Library::GameTime& gameTime);
		
		virtual void ScheduleToDestroy() ;
		virtual bool IsScheduledForDestruction()const;

		virtual void OnContactBegin(Box2DBehavior* other, b2Contact* contact) = 0;
		virtual void OnContactEnd(Box2DBehavior* other, b2Contact* contact) = 0;
		virtual void OnContactPreSolve(Box2DBehavior* other, b2Contact* contact, const b2Manifold* oldManifold) = 0;
		virtual void OnContactPostSolve(Box2DBehavior* other, b2Contact* contact, const b2ContactImpulse* impulse) = 0;

	protected:

		std::shared_ptr<Library::Box2DSprite> mSprite;

		BehaviorType mTag;

		bool mScheduledToDestroy;
	};

}