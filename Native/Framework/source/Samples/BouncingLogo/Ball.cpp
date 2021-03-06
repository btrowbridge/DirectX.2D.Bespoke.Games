#include "pch.h"
#include "Ball.h"
#include "SpriteBatch.h"
#include "BouncingLogoGame.h"

using namespace Library;
using namespace std;
using namespace Microsoft::WRL;
using namespace DirectX;

namespace BouncingLogo {
	const int Ball::mMinSpeed = 100;
	const int Ball::mMaxSpeed = 300;

	BouncingLogo::Ball::Ball(Library::Game & game) :
		DrawableGameComponent(game), mBounds(Rectangle::Empty)
	{
	}

	void BouncingLogo::Ball::Initialize()
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

		ResetBall();

		mSpriteBatch = make_unique<SpriteBatch>(mGame->Direct3DDeviceContext());

		//Sounds
		mAudioEngineComponent = reinterpret_cast<Library::AudioEngineComponent*>(
			mGame->Services().GetService(Library::AudioEngineComponent::TypeIdClass()));

		assert(mAudioEngineComponent != nullptr);

		auto& mAudioEngine = mAudioEngineComponent->AudioEngine();

		mSoundEffectPing = make_unique<SoundEffect>(mAudioEngine.get(), L"Content\\Audio\\Ping.wav");

		mSoundEffectPong = make_unique<SoundEffect>(mAudioEngine.get(), L"Content\\Audio\\Pong.wav");

		mSoundEffectWall = make_unique<SoundEffect>(mAudioEngine.get(), L"Content\\Audio\\Wall.wav");

		mSoundEffectScore = make_unique<SoundEffect>(mAudioEngine.get(), L"Content\\Audio\\Score.wav");

		mPlayer = mGame->As<BouncingLogoGame>()->getPlayer();
		mComputer = mGame->As<BouncingLogoGame>()->getComputer();
		mScoreBoard = mGame->As<BouncingLogoGame>()->getScoreBoard();
	}

	void BouncingLogo::Ball::Update(const Library::GameTime & gameTime)
	{
		if (mGame->As<BouncingLogoGame>()->GameState() == GameState::Play) {
			float elapsedTime = gameTime.ElapsedGameTimeSeconds().count();

			auto& mViewport = mGame->Viewport();

			XMFLOAT2 positionDelta(mVelocity.x * elapsedTime, mVelocity.y * elapsedTime);
			mBounds.X += static_cast<int>(std::round(positionDelta.x));
			mBounds.Y += static_cast<int>(std::round(positionDelta.y));

			if (mBounds.X + mBounds.Width > mViewport.Width && mVelocity.x > 0.0f)
			{
				mScoreBoard->PlayerScores();
				mSoundEffectScore->Play();
				ResetBall();
				return;
			}
			else if (mBounds.X < 0 && mVelocity.x < 0.0f)
			{
				mScoreBoard->ComputerScores();
				mSoundEffectScore->Play();
				ResetBall();
				return;
			}

			if (mBounds.Y + mBounds.Height > mViewport.Height && mVelocity.y > 0.0f)
			{
				mVelocity.y *= -1;
				mSoundEffectWall->Play();
			}
			else if (mBounds.Y < 0 && mVelocity.y < 0.0f)
			{
				mVelocity.y *= -1;
				mSoundEffectWall->Play();
			}

			if (mBounds.Intersects(mPlayer->Bounds()))
			{
				mSoundEffectPing->Play();
				if ((abs(mBounds.Left() - mPlayer->Bounds().Right()) >	//Bounce off the bottom or top
					abs(mBounds.Bottom() - mPlayer->Bounds().Top())) ||
					(abs(mBounds.Left() - mPlayer->Bounds().Right()) >
						abs(mBounds.Top() - mPlayer->Bounds().Bottom())))
				{
					if (mBounds.Center().Y - mPlayer->Bounds().Center().Y < 0) {
						mVelocity.y = abs(mVelocity.y) * -1;
						mBounds.Y = mPlayer->Bounds().Top() - mBounds.Height; //put above
					}
					else {
						mVelocity.y = abs(mVelocity.y);
						mBounds.Y = mPlayer->Bounds().Bottom(); //put below
					}
				}
				else {
					mVelocity.x *= -1;
					if (mBounds.Center().X > mPlayer->Bounds().Center().X) {
						mBounds.X = mPlayer->Bounds().Right();
					}
					else {
						mBounds.X = mPlayer->Bounds().Left() - mBounds.Width;
					}
				}

				if (mPlayer->Velocity().y != 0)
				{
					mVelocity.y = (mPlayer->Velocity().y + mVelocity.y) / 2; //transfer some paddle momentum
				}
			}
			else if (mBounds.Intersects(mComputer->Bounds()))
			{
				mSoundEffectPong->Play();
				if ((abs(mBounds.Right() - mComputer->Bounds().Left()) >	//Bounce off the bottom or top>
					abs(mBounds.Bottom() - mComputer->Bounds().Top())) ||
					(abs(mBounds.Right() - mComputer->Bounds().Left()) >
						abs(mBounds.Top() - mComputer->Bounds().Bottom())))
				{
					if (mBounds.Center().Y - mComputer->Bounds().Center().Y < 0) {
						mVelocity.y = abs(mVelocity.y) * -1;
						mBounds.Y = mComputer->Bounds().Top() - mBounds.Height; //put above
					}
					else {
						mVelocity.y = abs(mVelocity.y);
						mBounds.Y = mComputer->Bounds().Bottom(); //put below
					}
				}
				else {
					mVelocity.x *= -1;
					if (mBounds.Center().X > mComputer->Bounds().Center().X) {
						mBounds.X = mComputer->Bounds().Right();
					}
					else {
						mBounds.X = mComputer->Bounds().Left() - mBounds.Width;
					}
				}
				if (mComputer->Velocity().y != 0)
				{
					mVelocity.y = (mComputer->Velocity().y + mVelocity.y) / 2; //transfer some paddle momentum
				}
			}
		}
	}

	void BouncingLogo::Ball::Draw(const Library::GameTime & gameTime)
	{
		XMFLOAT2 position(static_cast<float>(mBounds.X), static_cast<float>(mBounds.Y));

		mSpriteBatch->Begin();
		mSpriteBatch->Draw(mTexture.Get(), position);
		mSpriteBatch->End();
	}

	void BouncingLogo::Ball::ResetBall()
	{
		Point mTextureHalfSize;
		mTextureHalfSize.X = mBounds.Width / 2;
		mTextureHalfSize.Y = mBounds.Height / 2;

		auto& mViewport = mGame->Viewport();

		Library::Rectangle viewportSize(static_cast<int>(mViewport.TopLeftX), static_cast<int>(mViewport.TopLeftY),
			static_cast<int>(mViewport.Width), static_cast<int>(mViewport.Height));
		Point center = viewportSize.Center();

		mBounds.X = center.X - mTextureHalfSize.X;
		mBounds.Y = center.Y - mTextureHalfSize.Y;

		mVelocity = RandomVelocity();
	}

	Library::Point BouncingLogo::Ball::Position()
	{
		return mBounds.Center();
	}

	DirectX::XMFLOAT2 BouncingLogo::Ball::Velocity()
	{
		return mVelocity;
	}

	DirectX::XMFLOAT2 Ball::RandomVelocity()
	{
		int range = mMaxSpeed - mMinSpeed;
		int xmag = rand() % range + mMinSpeed;
		int ymag = rand() % range + mMinSpeed;

		int xdir = (rand() % 2 == 1) ? 1 : -1;
		int ydir = (rand() % 2 == 1) ? 1 : -1;
		float dx = static_cast<float>(xmag * xdir);
		float dy = static_cast<float>(ymag * ydir);
		return XMFLOAT2(dx, dy);
	}
}