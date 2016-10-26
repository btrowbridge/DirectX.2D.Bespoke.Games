#include "pch.h"
#include "SpriteMaterial.h"

using namespace std;
using namespace DirectX;

namespace Library
{
	RTTI_DEFINITIONS(SpriteMaterial)

	const uint32_t SpriteMaterial::IndexCount = 6;

	SpriteMaterial::SpriteMaterial(Game& game, const shared_ptr<Texture2D>& texture) :
		Material(game),
		mTexture(texture), mSamplerState(SamplerStates::TrilinearClamp)
	{
	}

	void SpriteMaterial::Initialize()
	{
		Material::Initialize();

		mVertexShader = mGame->Content().Load<VertexShader>(L"Shaders\\SpriteVS.cso");
		mVertexShader->CreateInputLayout(mGame->Direct3DDevice(), const_cast<D3D11_INPUT_ELEMENT_DESC*>(VertexPositionTexture::InputElements), VertexPositionTexture::InputElementCount);
		mPixelShader = mGame->Content().Load<PixelShader>(L"Shaders\\SpritePS.cso");

		// Create a vertex buffer
		const VertexPositionTexture vertices[] =
		{
			//TODO: allow for tiled textures
			VertexPositionTexture(XMFLOAT4(-1.0f, -1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f)),
			VertexPositionTexture(XMFLOAT4(-1.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f)),
			VertexPositionTexture(XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 0.0f)),
			VertexPositionTexture(XMFLOAT4(1.0f, -1.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 1.0f)),
		};
	
		D3D11_BUFFER_DESC vertexBufferDesc = { 0 };
		vertexBufferDesc.ByteWidth = sizeof(VertexPositionTexture) * ARRAYSIZE(vertices);
		vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA vertexSubResourceData = { 0 };
		vertexSubResourceData.pSysMem = vertices;
		ThrowIfFailed(mGame->Direct3DDevice()->CreateBuffer(&vertexBufferDesc, &vertexSubResourceData, mVertexBuffer.ReleaseAndGetAddressOf()), "ID3D11Device::CreateBuffer() failed.");

		// Create and index buffer
		const uint32_t indices[] =
		{
			0, 1, 2,
			0, 2, 3
		};

		assert(ARRAYSIZE(indices) == IndexCount);

		D3D11_BUFFER_DESC indexBufferDesc = { 0 };
		indexBufferDesc.ByteWidth = sizeof(uint32_t) * IndexCount;
		indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

		D3D11_SUBRESOURCE_DATA indexSubResourceData = { 0 };
		indexSubResourceData.pSysMem = indices;
		ThrowIfFailed(mGame->Direct3DDevice()->CreateBuffer(&indexBufferDesc, &indexSubResourceData, mIndexBuffer.ReleaseAndGetAddressOf()), "ID3D11Device::CreateBuffer() failed.");

		// Create a constant buffer for passing to the vertex shader
		D3D11_BUFFER_DESC constantBufferDesc = { 0 };
		constantBufferDesc.ByteWidth = sizeof(XMFLOAT4X4);
		constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		ThrowIfFailed(mGame->Direct3DDevice()->CreateBuffer(&constantBufferDesc, nullptr, mConstantBuffer.ReleaseAndGetAddressOf()), "ID3D11Device::CreateBuffer() failed.");
	}

	uint32_t SpriteMaterial::VertexSize() const
	{
		return sizeof(VertexPositionTexture);
	}

	Microsoft::WRL::ComPtr<ID3D11SamplerState> SpriteMaterial::SamplerState() const
	{
		return mSamplerState;
	}

	void SpriteMaterial::SetSamplerState(const Microsoft::WRL::ComPtr<ID3D11SamplerState>& samplerState)
	{
		mSamplerState = samplerState;
	}

	void SpriteMaterial::Draw()
	{
		DrawIndexed(mVertexBuffer.Get(), mIndexBuffer.Get(), IndexCount);
	}

	void SpriteMaterial::BeginDraw()
	{
		Material::BeginDraw();

		auto direct3DDeviceContext = mGame->Direct3DDeviceContext();
		direct3DDeviceContext->VSSetConstantBuffers(0, 1, mConstantBuffer.GetAddressOf());
		direct3DDeviceContext->PSSetShaderResources(0, 1, mTexture->ShaderResourceView().GetAddressOf());
		direct3DDeviceContext->PSSetSamplers(0, 1, mSamplerState.GetAddressOf());
	}

	void SpriteMaterial::UpdateConstantBuffer(CXMMATRIX worldViewProjectionMatrix)
	{
		mGame->Direct3DDeviceContext()->UpdateSubresource(mConstantBuffer.Get(), 0, nullptr, reinterpret_cast<const float*>(worldViewProjectionMatrix.r), 0, 0);
	}
}