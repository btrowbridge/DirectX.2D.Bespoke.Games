#include "pch.h"
#include "Box.h"


using namespace Library;
namespace SimpleCollision{

	Box::Box(Library::Game& game) : DrawableGameComponent(game)
	{
	}

	Box::Box(Library::Game& game, const std::shared_ptr<Camera>& camera) : 
		DrawableGameComponent(game,camera)
	{
	}
	void Box::Initialize()
	{
		DrawableGameComponent::Initialize();
	}
	void Box::Update(const Library::GameTime & gameTime)
	{
		DrawableGameComponent::Update(gameTime);
	}
	void Box::Draw(const Library::GameTime & gameTime)
	{
		DrawableGameComponent::Update(gameTime);
	}
}