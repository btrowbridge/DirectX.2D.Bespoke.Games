#include "pch.h"
#include "PositionColorMaterial.h"

using namespace std;
using namespace Library;
using namespace DirectX;

namespace SimpleCollision
{
	RTTI_DEFINITIONS(PositionColorMaterial)

	PositionColorMaterial::PositionColorMaterial(Game& game) :
		Material(game, L"Content\\Shaders\\PositionColorVS.cso", L"Content\\Shaders\\PositionColorPS.cso")
	{
	}
	
	void PositionColorMaterial::Initialize()
	{
		// Create an input layout
		D3D11_INPUT_ELEMENT_DESC inputElementDescriptions[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		ThrowIfFailed(mGame->Direct3DDevice()->CreateInputLayout(inputElementDescriptions, ARRAYSIZE(inputElementDescriptions), &mCompiledVertexShader[0], mCompiledVertexShader.size(), mInputLayout.ReleaseAndGetAddressOf()), "ID3D11Device::CreateInputLayout() failed.");
	}

	uint32_t PositionColorMaterial::VertexSize() const
	{
		return sizeof(VertexPositionColor);
	}

	void PositionColorMaterial::BeginDraw()
	{
		ID3D11DeviceContext* direct3DDeviceContext = mGame->Direct3DDeviceContext();

		direct3DDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		direct3DDeviceContext->UpdateSubresource(mVSCBufferPerObject.Get(), 0, nullptr, &mVSCBufferPerObjectData, 0, 0);
		direct3DDeviceContext->VSSetConstantBuffers(0, 1, mVSCBufferPerObject.GetAddressOf());

		Material::BeginDraw();
	}
}