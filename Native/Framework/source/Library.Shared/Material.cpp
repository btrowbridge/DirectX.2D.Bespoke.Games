#include "pch.h"

using namespace std;
using namespace DirectX;

namespace Library
{
	RTTI_DEFINITIONS(Material)

	Material::Material(Game& game, const wstring& vertexShaderPath, const wstring& pixelShaderPath) :
		mGame(&game)
	{
		// Load a compiled vertex shader
		Utility::LoadBinaryFile(vertexShaderPath, mCompiledVertexShader);
		ThrowIfFailed(mGame->Direct3DDevice()->CreateVertexShader(&mCompiledVertexShader[0], mCompiledVertexShader.size(), nullptr, mVertexShader.ReleaseAndGetAddressOf()), "ID3D11Device::CreatedVertexShader() failed.");

		// Load a compiled pixel shader
		vector<char> compiledPixelShader;
		Utility::LoadBinaryFile(pixelShaderPath, compiledPixelShader);
		ThrowIfFailed(mGame->Direct3DDevice()->CreatePixelShader(&compiledPixelShader[0], compiledPixelShader.size(), nullptr, mPixelShader.ReleaseAndGetAddressOf()), "ID3D11Device::CreatedPixelShader() failed.");
	}

	void Material::Draw(ID3D11Buffer* vertexBuffer, uint32_t vertexCount, uint32_t startVertexLocation, uint32_t offset)
	{
		auto direct3DDeviceContext = mGame->Direct3DDeviceContext();

		BeginDraw();

		UINT stride = VertexSize();
		ID3D11Buffer* const vertexBuffers[] = { vertexBuffer };
		direct3DDeviceContext->IASetVertexBuffers(0, ARRAYSIZE(vertexBuffers), vertexBuffers, &stride, &offset);

		direct3DDeviceContext->Draw(vertexCount, startVertexLocation);

		EndDraw();
	}

	void Material::DrawIndexed(ID3D11Buffer* vertexBuffer, ID3D11Buffer* indexBuffer, uint32_t indexCount, DXGI_FORMAT format, uint32_t startIndexLocation, uint32_t baseVertexLocation, uint32_t vertexOffset, uint32_t indexOffset)
	{
		auto direct3DDeviceContext = mGame->Direct3DDeviceContext();

		BeginDraw();

		UINT stride = VertexSize();
		ID3D11Buffer* const vertexBuffers[] = { vertexBuffer };
		direct3DDeviceContext->IASetVertexBuffers(0, ARRAYSIZE(vertexBuffers), vertexBuffers, &stride, &vertexOffset);
		direct3DDeviceContext->IASetIndexBuffer(indexBuffer, format, indexOffset);

		direct3DDeviceContext->DrawIndexed(indexCount, startIndexLocation, baseVertexLocation);

		EndDraw();
	}

	void Material::BeginDraw()
	{
		mGame->Direct3DDeviceContext()->IASetInputLayout(mInputLayout.Get());
	}

	void Material::EndDraw()
	{
	}
}