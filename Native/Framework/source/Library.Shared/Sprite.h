#pragma once

#include "DrawableGameComponent.h"
#include "Transform2D.h"

namespace Library
{
	class SpriteMaterial;
	class Texture2D;

	class Sprite : public DrawableGameComponent
	{
		RTTI_DECLARATIONS(Sprite, DrawableGameComponent)

	public:
		Sprite(Game& game, const std::shared_ptr<Camera>& camera, const std::shared_ptr<Texture2D>& texture = nullptr);
		Sprite(const Sprite&) = default;
		Sprite& operator=(const Sprite&) = default;
		Sprite(Sprite&&) = default;
		Sprite& operator=(Sprite&&) = default;
		virtual ~Sprite() = default;

		std::shared_ptr<Texture2D> Texture() const;
		void SetTexture(const std::shared_ptr<Texture2D>& texture);

		const Transform2D& GetTransform() const;
		void SetTransform(const Transform2D& transform);

		virtual void Initialize() override;
		virtual void Draw(const GameTime& gameTime) override;

	protected:
		void UpdateMaterial();

		std::shared_ptr<SpriteMaterial> mMaterial;
		std::shared_ptr<Texture2D> mTexture;
		Transform2D mTransform;		
	};
}