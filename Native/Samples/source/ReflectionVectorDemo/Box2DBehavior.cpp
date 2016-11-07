#include "pch.h"
#include "Box2DBehavior.h"

using namespace Library;
using namespace std;

namespace Reflection2DGame {

	Box2DBehavior::Box2DBehavior(Library::Game & game, const std::shared_ptr<Library::Camera>& camera, std::shared_ptr<Library::Box2DSprite>& sprite, BehaviorType tag = BehaviorType::None) :
		DrawableGameComponent(game,camera), mSprite(sprite), mTag(tag), mScheduledToDestroy(false)
	{
	}
	Box2DBehavior::~Box2DBehavior()
	{

	}
	Library::Box2DSprite * Box2DBehavior::Sprite() const
	{
		return mSprite.get();
	}
	BehaviorType Box2DBehavior::Tag()
	{
		return mTag;
	}
	void Box2DBehavior::Initialize()
	{
		DrawableGameComponent::Initialize();
		if(mSprite != nullptr)
			mSprite->Initialize();
	}
	void Box2DBehavior::Update(const Library::GameTime & gameTime)
	{
		DrawableGameComponent::Update(gameTime);
		mSprite->Update(gameTime);
	}
	void Box2DBehavior::Draw(const Library::GameTime & gameTime)
	{
		DrawableGameComponent::Draw(gameTime);
		if (mSprite->Visible())
			mSprite->Draw(gameTime);
	}

	void Box2DBehavior::ScheduleToDestroy()
	{
		mScheduledToDestroy = true;
	}
	bool Box2DBehavior::IsScheduledForDestruction() const
	{
		return mScheduledToDestroy;
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