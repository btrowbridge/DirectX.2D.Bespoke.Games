#include "pch.h"
#include "SimpleCollisionGame.h"

using namespace std;
using namespace DirectX;
using namespace Library;
using namespace Microsoft::WRL;

namespace SimpleCollision
{
	const XMVECTORF32 SimpleCollisionGame::BackgroundColor = Colors::Black;

	SimpleCollisionGame::SimpleCollisionGame(function<void*()> getWindowCallback, function<void(SIZE&)> getRenderTargetSizeCallback) :
		Game(getWindowCallback, getRenderTargetSizeCallback)
	{
	}

	void SimpleCollisionGame::Initialize()
	{

		mKeyboard = make_shared<KeyboardComponent>(*this);
		mComponents.push_back(mKeyboard);
		mServices.AddService(KeyboardComponent::TypeIdClass(), mKeyboard.get());

		mPhysicsEngine = make_shared<Library::Box2DComponent>(*this);
		mComponents.push_back(mPhysicsEngine);
		mServices.AddService(Box2DComponent::TypeIdClass(), mPhysicsEngine.get());

		auto fpsCom = make_shared<FpsComponent>(*this);
		mComponents.push_back(fpsCom);

		auto mCamera = make_shared<OrthographicCamera>(*this);
		mComponents.push_back(mCamera);
		/*mCamera->SetPosition(0.0f, 0.0f, 50.0f);*/

		mDebugDraw = make_shared<DebugDraw>(*this, mCamera);
		mComponents.push_back(mDebugDraw);
		mDebugDraw->AppendFlags(b2Draw::e_shapeBit);
		
		mPhysicsEngine->World().SetDebugDraw(mDebugDraw.get());
		
		SpriteManager::Initialize(*this);

		Game::Initialize();
		

		AddPlatform(0.0f, -10.0f);
	}

	void SimpleCollisionGame::Update(const GameTime &gameTime)
	{
		if (mKeyboard->WasKeyPressedThisFrame(Keys::Space)) 
		{
			AddBox(0.0f, 10.0f);
		}

		if (mKeyboard->WasKeyPressedThisFrame(Keys::Escape))
		{
			Exit();
		}

		Game::Update(gameTime);
	}

	void SimpleCollisionGame::Draw(const GameTime &gameTime)
	{
		mDirect3DDeviceContext->ClearRenderTargetView(mRenderTargetView.Get(), reinterpret_cast<const float*>(&BackgroundColor));
		mDirect3DDeviceContext->ClearDepthStencilView(mDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		mPhysicsEngine->World().DrawDebugData();

		Game::Draw(gameTime);

		HRESULT hr = mSwapChain->Present(1, 0);

		// If the device was removed either by a disconnection or a driver upgrade, we must recreate all device resources.
		if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
		{
			HandleDeviceLost();
		}
		else
		{
			ThrowIfFailed(hr, "IDXGISwapChain::Present() failed.");
		}
	}

	void SimpleCollisionGame::Shutdown()
	{
	}

	void SimpleCollisionGame::AddBox(float x, float y)
	{
		// Define the dynamic body. We set its position and call the body factory.
		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		bodyDef.position.Set(x, y);
		b2Body* body = mPhysicsEngine->World().CreateBody(&bodyDef);

		// Define another box shape for our dynamic body.
		b2PolygonShape dynamicBox;
		dynamicBox.SetAsBox(1.0f, 1.0f);

		// Define the dynamic body fixture.
		b2FixtureDef fixtureDef;
		fixtureDef.shape = &dynamicBox;

		// Set the box density to be non-zero, so it will be dynamic.
		fixtureDef.density = 1.0f;

		// Override the default friction.
		fixtureDef.friction = 0.3f;

		// Add the shape to the body.
		body->CreateFixture(&fixtureDef);
	}

	void SimpleCollisionGame::AddPlatform(float x, float y)
	{
		// Define the ground body.
		b2BodyDef groundBodyDef;
		groundBodyDef.position.Set(x, y);

		// Call the body factory which allocates memory for the ground body
		// from a pool and creates the ground box shape (also from a pool).
		// The body is also added to the world.
		b2Body* groundBody = mPhysicsEngine->World().CreateBody(&groundBodyDef);

		// Define the ground box shape.
		b2PolygonShape groundBox;

		// The extents are the half-widths of the box.
		groundBox.SetAsBox(10.0f, 5.0f);

		// Add the ground fixture to the ground body.
		groundBody->CreateFixture(&groundBox, 0.0f);
	}

	void SimpleCollisionGame::AddWall(float x, float y)
	{
		// Define the ground body.
		b2BodyDef wallBodyDef;
		wallBodyDef.position.Set(x, y);
	
		// Call the body factory which allocates memory for the ground body
		// from a pool and creates the ground box shape (also from a pool).
		// The body is also added to the world.
		b2Body* wallBody = mPhysicsEngine->World().CreateBody(&wallBodyDef);

		// Define the ground box shape.
		b2EdgeShape wallEdge;

		// The extents are the half-widths of the box.
		//Set

		// Add the ground fixture to the ground body.
		wallBody->CreateFixture(&wallEdge, 0.0f);
	}

	void SimpleCollisionGame::Exit()
	{
		PostQuitMessage(0);
	}
}