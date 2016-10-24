#include "pch.h"
#include "GameObject.h"

using namespace std;

namespace Library
{
	RTTI_DEFINITIONS(GameObject)

	GameObject::GameObject(Game& game) :
		DrawableGameComponent(game)
	{
	}

	GameObject::GameObject(Game& game, const shared_ptr<Camera>& camera) :
		DrawableGameComponent(game, camera)
	{
	}
}