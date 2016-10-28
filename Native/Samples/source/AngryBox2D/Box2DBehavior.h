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
		Box2DBehavior(Library::Game& game, const std::shared_ptr<Library::Camera>& camera, Library::Box2DSprite* sprite, std::string tag);

		Library::Box2DSprite* Sprite() const;

		std::string Tag();

		virtual void Initialize();
		virtual void Update(const Library::GameTime& gameTime);
		virtual void Draw(const Library::GameTime& gameTime);
		
		virtual void Destroy();

		virtual void OnContactBegin(Box2DBehavior* other, b2Contact* contact);
		virtual void OnContactEnd(Box2DBehavior* other, b2Contact* contact);
		virtual void OnContactPreSolve(Box2DBehavior* other, b2Contact* contact, const b2Manifold* oldManifold);
		virtual void OnContactPostSolve(Box2DBehavior* other, b2Contact* contact, const b2ContactImpulse* impulse);

	protected:

		std::shared_ptr<Library::Box2DSprite> mSprite;

		std::string mTag;
	};

}