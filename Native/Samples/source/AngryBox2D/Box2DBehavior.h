#pragma once
#include <string>


namespace Library {
	class Box2DSprite;
	class Camera;
}

namespace AngryBox2DGame {

	class Box2DBehavior abstract : Library::DrawableGameComponent
	{
	public:
		Box2DBehavior(Library::Game& game, const std::shared_ptr<Library::Camera>& camera, Library::Box2DSprite* sprite);

		Library::Box2DSprite* Sprite() const;

		std::string Tag();

		virtual void Initialize();
		virtual void Update(const Library::GameTime& gameTime);
		virtual void Draw(const Library::GameTime& gameTime);
		
		virtual void OnContactBegin(b2Body* other, b2Contact* contact) = 0;
		virtual void OnContactEnd(b2Body* other, b2Contact* contact) = 0;
		virtual void OnContactPreSolve(b2Body* other, b2Contact* contact, const b2Manifold* oldManifold) = 0;
		virtual void OnContactPostSolve(b2Body* other, b2Contact* contact, const b2ContactImpulse* impulse) = 0;

	protected:

		std::shared_ptr<Library::Box2DSprite> mSprite;

		std::string mTag;
	};

}