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

		b2Body* Body();
		std::string Tag();

		virtual void Initialize();
		virtual void Update(const Library::GameTime& gameTime);
		virtual void Draw(const Library::GameTime& gameTime);
		
	protected:

		Library::Box2DSprite* mSprite;

		std::string mTag;

		virtual void OnContactBegin(b2Body* other, b2Contact* contact) = 0;
		virtual void OnContactEnd(b2Body* other, b2Contact* contact) = 0;
		virtual void OnContactPreSolve(b2Body* other, b2Contact* contact, const b2Manifold* oldManifold) = 0;
		virtual void OnContactPostSolve(b2Body* other, b2Contact* contact, const b2ContactImpulse* impulse) = 0;


	private:

		class ContactListener : b2ContactListener
		{
		public:
			ContactListener(Box2DBehavior* myObject) : b2ContactListener() {} 

			virtual void BeginContact(b2Contact* contact) 
			{
				Box2DBehavior* behaviorA = static_cast<Box2DBehavior*>(contact->GetFixtureA()->GetUserData());
				Box2DBehavior* behaviorB = static_cast<Box2DBehavior*>(contact->GetFixtureB()->GetUserData());

				if (behaviorA != nullptr) {
					behaviorA->OnContactBegin(behaviorB->Body(), contact);
				} 
				if (behaviorB != nullptr) {
					behaviorB->OnContactBegin(behaviorA->Body(), contact);
				}

			}

			virtual void EndContact(b2Contact* contact) 
			{ 
				Box2DBehavior* behaviorA = static_cast<Box2DBehavior*>(contact->GetFixtureA()->GetUserData());
				Box2DBehavior* behaviorB = static_cast<Box2DBehavior*>(contact->GetFixtureB()->GetUserData());

				if (behaviorA != nullptr) {
					behaviorA->OnContactEnd(behaviorB->Body(), contact);
				} 
				if (behaviorB != nullptr) {
					behaviorA->OnContactEnd(behaviorA->Body(), contact);
				}
			}

			virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
			{
				Box2DBehavior* behaviorA = static_cast<Box2DBehavior*>(contact->GetFixtureA()->GetUserData());
				Box2DBehavior* behaviorB = static_cast<Box2DBehavior*>(contact->GetFixtureB()->GetUserData());
			
				if (behaviorA != nullptr) {
					behaviorA->OnContactPreSolve(behaviorB->Body(), contact, oldManifold);
				} 
				if (behaviorB != nullptr) {
					behaviorB->OnContactPreSolve(behaviorA->Body(), contact,  oldManifold);
				}
			}

			virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
			{
				Box2DBehavior* behaviorA = static_cast<Box2DBehavior*>(contact->GetFixtureA()->GetUserData());
				Box2DBehavior* behaviorB = static_cast<Box2DBehavior*>(contact->GetFixtureB()->GetUserData());

				if (behaviorA != nullptr) {
					behaviorA->OnContactPostSolve(behaviorB->Body(), contact, impulse);
				} 
				if (behaviorB != nullptr) {
					behaviorB->OnContactPostSolve(behaviorA->Body(), contact, impulse);
				}
			}

		};

	private:
		ContactListener mContactListener;
	};

}