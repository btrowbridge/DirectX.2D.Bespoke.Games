#include "pch.h"
#include "Box2DBehavior.h"

namespace AngryBox2DGame {

	Box2DBehavior::Box2DBehavior(Library::Game & game, const std::shared_ptr<Library::Camera>& camera, Library::Box2DSprite* sprite) :
		DrawableGameComponent(game,camera), mSprite(sprite), mTag("None")
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