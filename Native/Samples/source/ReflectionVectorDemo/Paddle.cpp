#include "pch.h"
#include "Paddle.h"

using namespace Library;

namespace Reflection2DGame {

	const float32 Paddle::mRotationSpeed = 5.0f;

	Paddle::Paddle(Library::Game & game, const std::shared_ptr<Library::Camera>& camera, std::shared_ptr<Library::Box2DSprite>& sprite)
		: Box2DBehavior(game, camera, sprite, BehaviorType::Paddle), mKeyboard()
	{
	}
	void Paddle::Initialize()
	{
		Box2DBehavior::Initialize();
		mKeyboard = static_cast<KeyboardComponent*>(mGame->Services().GetService(KeyboardComponent::TypeIdClass()));
	}
	void Paddle::Update(const Library::GameTime & gameTime)
	{
		Box2DBehavior::Update(gameTime);

		if (mKeyboard->IsKeyDown(Keys::A))
		{
			mSprite->Body()->SetAngularVelocity(mRotationSpeed);
		}
		else if (mKeyboard->IsKeyDown(Keys::D))
		{
			mSprite->Body()->SetAngularVelocity(-mRotationSpeed);
		}
		else {
			mSprite->Body()->SetAngularVelocity(0);
		}
	}
	void Paddle::Draw(const Library::GameTime & gameTime)
	{
		Box2DBehavior::Draw(gameTime);
	}
	void Paddle::OnContactBegin(Box2DBehavior * other, b2Contact * contact)
	{
		UNREFERENCED_PARAMETER(other);
		UNREFERENCED_PARAMETER(contact);
	}
	void Paddle::OnContactEnd(Box2DBehavior * other, b2Contact * contact)
	{
		UNREFERENCED_PARAMETER(other);
		UNREFERENCED_PARAMETER(contact);
	}
	void Paddle::OnContactPreSolve(Box2DBehavior * other, b2Contact * contact, const b2Manifold * oldManifold)
	{
		UNREFERENCED_PARAMETER(other);
		UNREFERENCED_PARAMETER(contact);
		UNREFERENCED_PARAMETER(oldManifold);
	}
	void Paddle::OnContactPostSolve(Box2DBehavior * other, b2Contact * contact, const b2ContactImpulse * impulse)
	{
		UNREFERENCED_PARAMETER(other);
		UNREFERENCED_PARAMETER(contact);
		UNREFERENCED_PARAMETER(impulse);
	}
}
