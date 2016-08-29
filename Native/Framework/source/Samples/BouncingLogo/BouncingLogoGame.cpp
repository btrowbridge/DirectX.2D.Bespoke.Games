#include "pch.h"
#include "BouncingLogoGame.h"

using namespace std;
using namespace DirectX;
using namespace Library;
using namespace Microsoft::WRL;

namespace BouncingLogo
{
	const XMVECTORF32 BouncingLogoGame::BackgroundColor = Colors::CornflowerBlue;

	BouncingLogoGame::BouncingLogoGame(function<void*()> getWindowCallback, function<void(SIZE&)> getRenderTargetSizeCallback) :
		Game(getWindowCallback, getRenderTargetSizeCallback), mBounds(Library::Rectangle::Empty)
	{
	}

	void BouncingLogoGame::Initialize()
	{
		mKeyboard = make_shared<KeyboardComponent>(*this);
		mComponents.push_back(mKeyboard);
		mServices.AddService(KeyboardComponent::TypeIdClass(), mKeyboard.get());

		// Load a texture
		ComPtr<ID3D11Resource> textureResource;
		wstring textureName = L"Content\\Textures\\Ball.png";

		ThrowIfFailed(CreateWICTextureFromFile(mDirect3DDevice.Get(), textureName.c_str(), 
			textureResource.ReleaseAndGetAddressOf(), mTexture.ReleaseAndGetAddressOf()),
			"CreateWICTextureFromFile() Failed.");

		ComPtr<ID3D11Texture2D> texture;
		ThrowIfFailed(textureResource.As(&texture), "Invalid ID3D11 texture resource");

		mBounds = TextureHelper::GetTextureBounds(texture.Get());

		Point mTextureHalfSize;
		mTextureHalfSize.X = mBounds.Width / 2;
		mTextureHalfSize.Y = mBounds.Height / 2;

		Library::Rectangle viewportSize(static_cast<int>(mViewport.TopLeftX), static_cast<int>(mViewport.TopLeftY),
			static_cast<int>(mViewport.Width), static_cast<int>(mViewport.Height));
		Point center = viewportSize.Center();

		mBounds.X = center.X - mTextureHalfSize.X;
		mBounds.Y = center.Y - mTextureHalfSize.Y;

		mVelocity.x = 40;
		mVelocity.y = 20;

		//mSpriteBatch = make_unique<SpriteBatch>();

		Game::Initialize();
	}

	void BouncingLogoGame::Update(const GameTime &gameTime)
	{
		if (mKeyboard->WasKeyPressedThisFrame(Keys::Escape))
		{
			Exit();
		}

		float elapsedTime = gameTime.ElapsedGameTimeSeconds().count();
		//mBounds.X += static_cast<int>(mVelocity.X) * elapsedTime;
		//mBounds.Y += static_cast<int>(mVelocity.Y) * elapsedTime;

		XMFLOAT2 positionDelta(mVelocity.x * elapsedTime, mVelocity.y * elapsedTime);
		mBounds.X += static_cast<int>(std::round(positionDelta.x));
		mBounds.Y += static_cast<int>(std::round(positionDelta.y));

		if (mBounds.X + mBounds.Width > mViewport.Width && mVelocity.x > 0.0f)
		{
			mVelocity.x *= -1;
		} 
		else if (mBounds.X < 0 && mVelocity.x < 0.0f)
		{
			mVelocity.x *= -1;
		}

		if (mBounds.Y + mBounds.Height > mViewport.Height && mVelocity.y > 0.0f)
		{
			mVelocity.y *= -1;
		} 
		else if (mBounds.Y < 0 && mVelocity.y < 0.0f)
		{
			mVelocity.y *= -1;
		}


		Game::Update(gameTime);
	}

	void BouncingLogoGame::Draw(const GameTime &gameTime)
	{
		mDirect3DDeviceContext->ClearRenderTargetView(mRenderTargetView.Get(), reinterpret_cast<const float*>(&BackgroundColor));
		mDirect3DDeviceContext->ClearDepthStencilView(mDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		XMFLOAT2 position(0, 0);

		SpriteManager::DrawTexture2D(mTexture.Get(), position);

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

	void BouncingLogoGame::Shutdown()
	{
	
	}

	void BouncingLogoGame::Exit()
	{
		PostQuitMessage(0);
	}
}