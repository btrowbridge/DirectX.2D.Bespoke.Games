#include "pch.h"
#include "Box2DBehavior.h"

namespace AngryBox2DGame {

	Box2DBehavior::Box2DBehavior(Library::Game & game, const std::shared_ptr<Library::Camera>& camera, Library::Box2DSprite* sprite) :
		DrawableGameComponent(game,camera), mContactListener(this), mSprite(sprite), mTag("No Tag Set")
	{
	}
	b2Body * Box2DBehavior::Body()
	{
		return mSprite->Body();
	}
	std::string Box2DBehavior::Tag()
	{
		return mTag;
	}
	void Box2DBehavior::Initialize()
	{
		DrawableGameComponent::Initialize();

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
		mSprite->Draw(gameTime);
	}
}