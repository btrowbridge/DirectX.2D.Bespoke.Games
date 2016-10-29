#include "pch.h"
#include "BreakableBox.h"
#include <numeric>

using namespace std;
using namespace Library;
using namespace DirectX;

namespace AngryBox2DGame {
	BreakableBox::BreakableBox(Library::Game & game, const std::shared_ptr<Library::Camera>& camera, std::shared_ptr<Library::Box2DSprite>& sprite, float startHealth) :
		Box2DBehavior(game,camera,sprite,BehaviorType::BreakableBox), mHealth(startHealth)
	{

	}
	void BreakableBox::Initialize()
	{
		Box2DBehavior::Initialize();
		mHealthFont = make_unique<SpriteFont>(mGame->Direct3DDevice(), L"Content\\Fonts\\Arial_14_Regular.spritefont");		

	}
	void BreakableBox::Update(const Library::GameTime & gameTime)
	{
		Box2DBehavior::Update(gameTime);
	}
	void BreakableBox::Draw(const Library::GameTime & gameTime)
	{
		Box2DBehavior::Draw(gameTime);

		//wostringstream healthText;
		//healthText << "HP:" << mHealth;

		//XMFLOAT2 textPosition(Sprite()->Body()->GetPosition().x, Sprite()->Body()->GetPosition().y + 5);
		//SpriteManager::DrawString(mHealthFont, healthText.str().c_str(), XMFLOAT2(textPositionProjection.x, textPositionProjection.y));
		
	}

	void BreakableBox::OnContactBegin(Box2DBehavior * other, b2Contact * contact)
	{		
		UNREFERENCED_PARAMETER(other);
		UNREFERENCED_PARAMETER(contact);
	}

	void BreakableBox::OnContactEnd(Box2DBehavior * other, b2Contact * contact)
	{
		UNREFERENCED_PARAMETER(other);
		UNREFERENCED_PARAMETER(contact);

	}

	void BreakableBox::OnContactPreSolve(Box2DBehavior * other, b2Contact * contact, const b2Manifold * oldManifold)
	{
		UNREFERENCED_PARAMETER(other);
		UNREFERENCED_PARAMETER(contact);
		UNREFERENCED_PARAMETER(oldManifold);
	}

	void BreakableBox::OnContactPostSolve(Box2DBehavior * other, b2Contact * contact, const b2ContactImpulse * impulse)
	{	
		UNREFERENCED_PARAMETER(other);
		UNREFERENCED_PARAMETER(contact);
		//Calculate damages from impulse forces
		float calculatedDamage = std::round(impulse->normalImpulses[0]);
		TakeDamage(calculatedDamage);
	}
	void BreakableBox::TakeDamage(float damageAmount)
	{
		mHealth -= damageAmount;
		if (mHealth <= 0) {
			ScheduleToDestroy();
		}
	}
}