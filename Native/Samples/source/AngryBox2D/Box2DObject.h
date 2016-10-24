#pragma once
#include "DrawableGameComponent.h"

namespace AngryBox2DGame {
	
	class Box2DObject :
		public Library::DrawableGameComponent
	{
		RTTI_DECLARATIONS(Box2DObject, Library::DrawableGameComponent)

	public:
		Box2DObject(Library::Game& game, std::string defFile);


		void Initialize();
		void Update(Library::GameTime gameTime);
		void Draw(Library::GameTime gameTime);

		const DirectX::XMFLOAT2 Position() const;
		const float32 Rotation() const;

		const void* Info() const;

	protected:

		void LoadDefinitionsFromFile();

	private:

		b2Body* mBody;
		b2Fixture* mFixture;
		b2Shape* mShape;

		b2World *mWorld;

		std::string mDefFileName;

	};

}