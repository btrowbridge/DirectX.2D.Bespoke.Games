#include "pch.h"
#include "Box2DObject.h"

using namespace Library;
using namespace std;
using namespace DirectX;

namespace AngryBox2D{
	Box2DObject::Box2DObject(Library::Game & game, string defFile) :
		DrawableGameComponent(game),mBody(),mFixture(),mShape(),mDefFileName(defFile)
	{
	}
	void Box2DObject::Initialize()
	{
		LoadDefinitionsFromFile();

		DrawableGameComponent::Initialize();
	}
	void Box2DObject::Update(Library::GameTime gameTime)
	{
		DrawableGameComponent::Update(gameTime);
	}
	void Box2DObject::Draw(Library::GameTime gameTime)
	{
		//TODO
		DrawableGameComponent::Draw(gameTime);
	}

	const DirectX::XMFLOAT2 Box2DObject::Position() const
	{
		return XMFLOAT2(mBody->GetPosition().x,mBody->GetPosition().y);
	}

	const float32 Box2DObject::Rotation() const
	{
		//TODO:
		return float32();
	}

	const void* Box2DObject::Info() const
	{
		return this;
	}
	void Box2DObject::LoadDefinitionsFromFile()
	{
		
		
	}
}


