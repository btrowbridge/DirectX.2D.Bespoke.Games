#include "pch.h"
#include "Box2DBehavior.h"

using namespace Library;
using namespace std;

namespace AngryBox2DGame {

	Box2DBehavior::Box2DBehavior(Library::Game & game, const std::shared_ptr<Library::Camera>& camera, Library::Box2DSprite* sprite, std::string tag = "None") :
		DrawableGameComponent(game,camera), mSprite(sprite), mTag(tag)
	{
	}
	Library::Box2DSprite * Box2DBehavior::Sprite() const
	{
		return mSprite.get();
	}
	std::string Box2DBehavior::Tag()
	{
		return mTag;
	}
	void Box2DBehavior::Initialize()
	{
		DrawableGameComponent::Initialize();
	}
	void Box2DBehavior::Update(const Library::GameTime & gameTime)
	{
		DrawableGameComponent::Update(gameTime);
	}
	void Box2DBehavior::Draw(const Library::GameTime & gameTime)
	{
		DrawableGameComponent::Draw(gameTime);
	}
	void Box2DBehavior::Destroy()
	{
		auto physicsEngine = reinterpret_cast<Box2DComponent*>(mGame->Services().GetService(Box2DComponent::TypeIdClass()));
		physicsEngine->BuryBody(*mSprite->Body());
		delete this;
	}
	void Box2DBehavior::OnContactBegin(Box2DBehavior * other, b2Contact * contact)
	{
		UNREFERENCED_PARAMETER(other);
		UNREFERENCED_PARAMETER(contact);
	}
	void Box2DBehavior::OnContactEnd(Box2DBehavior * other, b2Contact * contact)
	{
		UNREFERENCED_PARAMETER(other);
		UNREFERENCED_PARAMETER(contact);
	}
	void Box2DBehavior::OnContactPreSolve(Box2DBehavior * other, b2Contact * contact, const b2Manifold * oldManifold)
	{
		UNREFERENCED_PARAMETER(other);
		UNREFERENCED_PARAMETER(contact);
		UNREFERENCED_PARAMETER(oldManifold);
	}
	void Box2DBehavior::OnContactPostSolve(Box2DBehavior * other, b2Contact * contact, const b2ContactImpulse * impulse)
	{
		UNREFERENCED_PARAMETER(other);
		UNREFERENCED_PARAMETER(contact);
		UNREFERENCED_PARAMETER(impulse);
	}
}