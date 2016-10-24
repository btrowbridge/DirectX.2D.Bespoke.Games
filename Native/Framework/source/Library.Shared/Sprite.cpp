#include "pch.h"
#include "Sprite.h"

using namespace std;
using namespace DirectX;

namespace Library
{
	RTTI_DEFINITIONS(Sprite)

	Sprite::Sprite(Game& game, const shared_ptr<Camera>& camera, const shared_ptr<Texture2D>& texture) :
		DrawableGameComponent(game, camera),
		mTexture(texture)
	{
	}

	shared_ptr<Texture2D> Sprite::Texture() const
	{
		return mTexture;
	}

	void Sprite::SetTexture(const shared_ptr<Texture2D>& texture)
	{
		mTexture = texture;
	}

	const Transform2D& Sprite::GetTransform() const
	{
		return mTransform;
	}

	void Sprite::SetTransform(const Transform2D& transform)
	{
		mTransform = transform;
	}

	void Sprite::Initialize()
	{
		mMaterial = make_shared<SpriteMaterial>(*mGame, mTexture);
		mMaterial->Initialize();

		using namespace std::placeholders;
		mMaterial->SetUpdateMaterialCallback(bind(&Sprite::UpdateMaterial, this));
	}

	void Sprite::Draw(const GameTime& gameTime)
	{
		UNREFERENCED_PARAMETER(gameTime);
		
		mMaterial->Draw();
	}

	void Sprite::UpdateMaterial()
	{
		XMMATRIX wvp = mTransform.WorldMatrix() * mCamera->ViewProjectionMatrix();
		wvp = XMMatrixTranspose(wvp);
		mMaterial->UpdateConstantBuffer(wvp);
	}
}