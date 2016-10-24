#include "pch.h"
#include "AnimatedSprite.h"

using namespace std;
using namespace DirectX;

namespace Library
{
	RTTI_DEFINITIONS(AnimatedSprite)

	AnimatedSprite::AnimatedSprite(Game& game, const shared_ptr<Camera>& camera) :
		DrawableGameComponent(game, camera)
	{
	}
}