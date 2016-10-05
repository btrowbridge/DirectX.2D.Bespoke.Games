#pragma once

#include <wrl.h>
#include <string>
#include <cstdint>
#include <vector>
#include <d3d11_2.h>
#include "RTTI.h"

namespace Library
{
	class Game;

	class Material : public RTTI
	{
		RTTI_DECLARATIONS(Material, RTTI)

	public:
		Material(Game& game, const std::wstring& vertexShaderPath, const std::wstring& pixelShaderPath);

		virtual void Initialize() = 0;
		virtual void Draw(ID3D11Buffer* vertexBuffer, std::uint32_t vertexCount, std::uint32_t startVertexLocation = 0, std::uint32_t offset = 0);
		virtual void DrawIndexed(ID3D11Buffer* vertexBuffer, ID3D11Buffer* indexBuffer, std::uint32_t indexCount, DXGI_FORMAT format = DXGI_FORMAT_R32_UINT, std::uint32_t startIndexLocation = 0, std::uint32_t baseVertexLocation = 0, std::uint32_t vertexOffset = 0, std::uint32_t indexOffset = 0);
		virtual std::uint32_t VertexSize() const = 0;

	protected:
		virtual void BeginDraw();
		virtual void EndDraw();

		Game* mGame;
		std::vector<char> mCompiledVertexShader;
		Microsoft::WRL::ComPtr<ID3D11VertexShader> mVertexShader;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> mPixelShader;
		Microsoft::WRL::ComPtr<ID3D11InputLayout> mInputLayout;
	};
}
