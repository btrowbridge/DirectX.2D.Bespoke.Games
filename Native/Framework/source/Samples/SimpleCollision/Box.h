#pragma once
#include "DrawableGameComponent.h"

namespace SimpleCollision {

	class Box :
		public Library::DrawableGameComponent
	{
	public:
		Box(Library::Game& game);
		Box(Library::Game& game, const std::shared_ptr<Library::Camera>& camera);

		virtual void Initialize() override;
		virtual void Update(const Library::GameTime& gameTime) override;
		virtual void Draw(const Library::GameTime& gameTime) override;

	private:
		b2World* mWorld;

	};

}

