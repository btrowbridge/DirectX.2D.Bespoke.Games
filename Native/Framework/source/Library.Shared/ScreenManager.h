#pragma once

#include "DrawableGameComponent.h"
#include "GameScreen.h"
#include <d3d11_2.h>
#include <DirectXMath.h>
#include <vector>
#include <memory>

namespace Library
{
	class ScreenInputManager;
	class IServiceProvider;

	class ScreenManager final : public DrawableGameComponent
	{
		RTTI_DECLARATIONS(ScreenManager, DrawableGameComponent)

	public:
		ScreenManager(Game& game);
		ScreenManager(Game& game, const std::shared_ptr<Camera>& camera);		

		DirectX::XMFLOAT2 ScreenCenter() const;
		const std::vector<std::shared_ptr<GameScreen>>& Screens() const;
		const Library::IServiceProvider& Services() const;

		virtual void Initialize() override;
		virtual void Update(const GameTime& gameTime) override;
		virtual void Draw(const GameTime& gameTime) override;

		void AddScreen(std::shared_ptr<GameScreen> screen);
		void AddScreens(const std::vector<std::shared_ptr<GameScreen>>& screens);
		void RemoveScreen(GameScreen& screen);
		void RemoveAllScreens();
		bool ContainsScreen(std::shared_ptr<GameScreen> screen) const;
		void FadeScreenToBlack(float alpha);

	private:
		std::vector<std::shared_ptr<GameScreen>> mScreens;
		std::vector<std::shared_ptr<GameScreen>> mScreensToUpdate;
		std::shared_ptr<ScreenInputManager> mScreenInputManager;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mBlankTexture;		
	};
}