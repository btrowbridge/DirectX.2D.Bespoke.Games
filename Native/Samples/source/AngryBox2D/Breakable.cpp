#include "pch.h"
#include "Breakable.h"


using namespace std;
using namespace Library;
using namespace DirectX;

namespace AngryBox2DGame {
	const float Breakable::mDamageThreshold = 1.0f;

	Breakable::Breakable(Library::Game & game, const std::shared_ptr<Library::Camera>& camera, std::shared_ptr<Library::Box2DSprite>& sprite, float startHealth, int scorevalue) :
		Box2DBehavior(game,camera,sprite,BehaviorType::Breakable), mHealth(startHealth), mScoreValue(scorevalue)
	{

	}
	void Breakable::Initialize()
	{
		Box2DBehavior::Initialize();
		mHealthFont = make_unique<SpriteFont>(mGame->Direct3DDevice(), L"Content\\Fonts\\Arial_14_Regular.spritefont");		

	}
	void Breakable::Update(const Library::GameTime & gameTime)
	{
		Box2DBehavior::Update(gameTime);
		if (mSprite->Body()->GetPosition().Length() > 50.0f) {
			ScheduleToDestroy();
		}
	}
	void Breakable::Draw(const Library::GameTime & gameTime)
	{
		Box2DBehavior::Draw(gameTime);

		//wostringstream healthText;
		//healthText << "HP:" << mHealth;

		//XMFLOAT2 textPosition(Sprite()->Body()->GetPosition().x, Sprite()->Body()->GetPosition().y + 5);
		//SpriteManager::DrawString(mHealthFont, healthText.str().c_str(), XMFLOAT2(textPositionProjection.x, textPositionProjection.y));
		
	}

	void Breakable::OnContactBegin(Box2DBehavior * other, b2Contact * contact)
	{		
		UNREFERENCED_PARAMETER(other);
		UNREFERENCED_PARAMETER(contact);
	}

	void Breakable::OnContactEnd(Box2DBehavior * other, b2Contact * contact)
	{
		UNREFERENCED_PARAMETER(other);
		UNREFERENCED_PARAMETER(contact);

	}

	void Breakable::OnContactPreSolve(Box2DBehavior * other, b2Contact * contact, const b2Manifold * oldManifold)
	{
		UNREFERENCED_PARAMETER(other);
		UNREFERENCED_PARAMETER(contact);
		UNREFERENCED_PARAMETER(oldManifold);
	}

	void Breakable::OnContactPostSolve(Box2DBehavior * other, b2Contact * contact, const b2ContactImpulse * impulse)
	{	
		UNREFERENCED_PARAMETER(other);
		UNREFERENCED_PARAMETER(contact);
		//Calculate damages from impulse forces
		float calculatedDamage = std::round(impulse->normalImpulses[0]);
		TakeDamage(calculatedDamage);
	}
	int Breakable::ScoreValue()
	{
		return mScoreValue;
	}
	void Breakable::TakeDamage(float damageAmount)
	{	
		if (damageAmount > mDamageThreshold)
			mHealth -= damageAmount;
		if (mHealth <= 0) {
			ScheduleToDestroy();
		}
	}
}