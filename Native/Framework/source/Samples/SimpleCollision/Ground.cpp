#include "pch.h"
#include "Ground.h"

using namespace Library;
namespace SimpleCollision{

	Ground::Ground(Library::Game& game) : DrawableGameComponent(game)
	{
	}

	Ground::Ground(Library::Game& game, const std::shared_ptr<Camera>& camera) : 
		DrawableGameComponent(game,camera)
	{
	}
	void Ground::Initialize()
	{
		DrawableGameComponent::Initialize();
	}
	void Ground::Update(const Library::GameTime & gameTime)
	{
		DrawableGameComponent::Update(gameTime);
	}
	void Ground::Draw(const Library::GameTime & gameTime)
	{
		DrawableGameComponent::Update(gameTime);
	}
}