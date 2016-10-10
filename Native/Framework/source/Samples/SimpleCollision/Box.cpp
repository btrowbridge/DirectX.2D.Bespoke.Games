#include "pch.h"
#include "Box.h"

using namespace std;
using namespace Library;
using namespace DirectX;
using namespace Microsoft::WRL;


namespace SimpleCollision{

	Box::Box(Library::Game& game) : DrawableGameComponent(game), mStartingPosition(), mBounds(Rectangle::Empty)
	{
	}

	Box::Box(Library::Game& game, const std::shared_ptr<Camera>& camera) : 
		DrawableGameComponent(game,camera), mStartingPosition(), mBounds(Rectangle::Empty)
	{
	}
	Box::Box(Library::Game & game, const std::shared_ptr<Library::Camera>& camera, DirectX::XMFLOAT2 startPosition) :
		DrawableGameComponent(game,camera), mStartingPosition(startPosition), mBounds(Rectangle::Empty)
	{
	}
	void Box::Initialize()
	{
		// Load a texture
		ComPtr<ID3D11Resource> textureResource;
		wstring textureName = L"Content\\Textures\\Ball.png";

		ThrowIfFailed(CreateWICTextureFromFile(mGame->Direct3DDevice(), textureName.c_str(),
			textureResource.ReleaseAndGetAddressOf(), mTexture.ReleaseAndGetAddressOf()),
			"CreateWICTextureFromFile() Failed.");

		ComPtr<ID3D11Texture2D> texture;
		ThrowIfFailed(textureResource.As(&texture), "Invalid ID3D11 texture resource");

		mBounds = TextureHelper::GetTextureBounds(texture.Get());

		mSpriteBatch = make_unique<SpriteBatch>(mGame->Direct3DDeviceContext());

		mPhysics = reinterpret_cast<Box2DComponent*>(mGame->As<SimpleCollisionGame>()->Services().GetService(Box2DComponent::TypeIdClass()));

		b2BodyDef boxBodyDef;
		boxBodyDef.position.x = mStartingPosition.x;
		boxBodyDef.position.y = mStartingPosition.y;	

		mBody = mPhysics->World().CreateBody(&boxBodyDef);
		
		b2PolygonShape boxShape;

		boxShape.SetAsBox(mBounds.Width / 2, mBounds.Height / 2);

		b2FixtureDef boxFixtureDef;

		mFixture = mBody->CreateFixture(&boxShape,static_cast<float32>(mBounds.Height*mBounds.Width));

		DrawableGameComponent::Initialize();
	}
	void Box::Update(const Library::GameTime & gameTime)
	{
		DrawableGameComponent::Update(gameTime);
	}
	void Box::Draw(const Library::GameTime & gameTime)
	{
		mBounds.X = mBody->GetPosition().x;
		mBounds.Y = mBody->GetPosition().y;
		XMFLOAT2 position(static_cast<float>(mBounds.X), static_cast<float>(mBounds.Y));
		float32 rotation(mBody->GetAngle());

		SpriteManager::DrawTexture2D(mTexture.Get(), position);

		DrawableGameComponent::Update(gameTime);
	}
}