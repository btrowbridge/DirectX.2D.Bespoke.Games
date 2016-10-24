#pragma once

#include "RTTI.h"
#include <wrl.h>
#include <string>
#include <cstdint>
#include <vector>
#include <functional>
#include <d3d11_2.h>
#include <DirectXMath.h>

namespace Library
{
	class Game;
	class VertexShader;
	class PixelShader;

	class Material : public RTTI
	{
		RTTI_DECLARATIONS(Material, RTTI)

	public:
		Material(Game& game, const std::shared_ptr<VertexShader>& vertexShader = nullptr, const std::shared_ptr<PixelShader>& pixelShader = nullptr, D3D_PRIMITIVE_TOPOLOGY topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		D3D_PRIMITIVE_TOPOLOGY Topology() const;
		void SetTopology(D3D_PRIMITIVE_TOPOLOGY topology);

		std::shared_ptr<VertexShader> GetVertexShader() const;
		void SetVertexShader(const std::shared_ptr<VertexShader>& vertexShader);

		std::shared_ptr<PixelShader> GetPixelShader() const;		
		void SetPixelShader(const std::shared_ptr<PixelShader>& pixelShader);

		std::function<void()> DrawCallback() const;
		void SetDrawCallback(std::function<void()> callback);

		std::function<void()> UpdateMaterialCallback() const;
		void SetUpdateMaterialCallback(std::function<void()> callback);

		virtual void Initialize();
		virtual void Draw();
		virtual void Draw(ID3D11Buffer* vertexBuffer, std::uint32_t vertexCount, std::uint32_t startVertexLocation = 0, std::uint32_t offset = 0);
		virtual void DrawIndexed(ID3D11Buffer* vertexBuffer, ID3D11Buffer* indexBuffer, std::uint32_t indexCount, DXGI_FORMAT format = DXGI_FORMAT_R32_UINT, std::uint32_t startIndexLocation = 0, std::uint32_t baseVertexLocation = 0, std::uint32_t vertexOffset = 0, std::uint32_t indexOffset = 0);
		virtual std::uint32_t VertexSize() const = 0;

	protected:
		virtual void BeginDraw();
		virtual void EndDraw();

		Game* mGame;
		D3D_PRIMITIVE_TOPOLOGY mTopology;
		std::shared_ptr<VertexShader> mVertexShader;
		std::shared_ptr<PixelShader> mPixelShader;
		std::function<void()> mDrawCallback;
		std::function<void()> mUpdateMaterialCallback;
	};
}
