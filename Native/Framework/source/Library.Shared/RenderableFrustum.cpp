#include "pch.h"
#include "RenderableFrustum.h"

using namespace std;
using namespace DirectX;

namespace Library
{
    RTTI_DEFINITIONS(RenderableFrustum)

	const XMFLOAT4 RenderableFrustum::DefaultColor(&Colors::Green[0]);

	const uint32_t RenderableFrustum::FrustumVertexCount = 8;
	const uint32_t RenderableFrustum::FrustumPrimitiveCount = 12;
	const uint32_t RenderableFrustum::FrustumIndicesPerPrimitive = 2;
	const uint32_t RenderableFrustum::FrustumIndexCount = FrustumPrimitiveCount * FrustumIndicesPerPrimitive;
	const uint16_t RenderableFrustum::FrustumIndices[] = {
														// Near plane lines
														0, 1,
														1, 2,
														2, 3,
														3, 0,

														// Sides
														0, 4,
														1, 5,
														2, 6,
														3, 7,

														// Far plane lines
														4, 5,
														5, 6,
														6, 7,
														7, 4
													};

	RenderableFrustum::RenderableFrustum(Game& game, const shared_ptr<Camera>& camera, const XMFLOAT4& color) :
		DrawableGameComponent(game, camera),
		mColor(color), mPosition(Vector3Helper::Zero), mDirection(Vector3Helper::Forward), mUp(Vector3Helper::Up),
		mRight(Vector3Helper::Right), mWorldMatrix(MatrixHelper::Identity)
    {
    }

	const XMFLOAT3& RenderableFrustum::Position() const
    {
        return mPosition;
    }

    const XMFLOAT3& RenderableFrustum::Direction() const
    {
        return mDirection;
    }
    
    const XMFLOAT3& RenderableFrustum::Up() const
    {
        return mUp;
    }

    const XMFLOAT3& RenderableFrustum::Right() const
    {
        return mRight;
    }

    XMVECTOR RenderableFrustum::PositionVector() const
    {
        return XMLoadFloat3(&mPosition);
    }

    XMVECTOR RenderableFrustum::DirectionVector() const
    {
        return XMLoadFloat3(&mDirection);
    }

    XMVECTOR RenderableFrustum::UpVector() const
    {
        return XMLoadFloat3(&mUp);
    }
    
    XMVECTOR RenderableFrustum::RightVector() const
    {
        return XMLoadFloat3(&mRight);
    }

	void RenderableFrustum::SetPosition(float x, float y, float z)
    {
        XMVECTOR position = XMVectorSet(x, y, z, 1.0f);
        SetPosition(position);
    }

    void RenderableFrustum::SetPosition(FXMVECTOR position)
    {
        XMStoreFloat3(&mPosition, position);
    }

    void RenderableFrustum::SetPosition(const XMFLOAT3& position)
    {
        mPosition = position;
    }

	void RenderableFrustum::ApplyRotation(CXMMATRIX transform)
    {
        XMVECTOR direction = XMLoadFloat3(&mDirection);
        XMVECTOR up = XMLoadFloat3(&mUp);
        
        direction = XMVector3TransformNormal(direction, transform);
        direction = XMVector3Normalize(direction);

        up = XMVector3TransformNormal(up, transform);
        up = XMVector3Normalize(up);

        XMVECTOR right = XMVector3Cross(direction, up);
        up = XMVector3Cross(right, direction);

        XMStoreFloat3(&mDirection, direction);
        XMStoreFloat3(&mUp, up);
        XMStoreFloat3(&mRight, right);
    }

    void RenderableFrustum::ApplyRotation(const XMFLOAT4X4& transform)
    {
        XMMATRIX transformMatrix = XMLoadFloat4x4(&transform);
        ApplyRotation(transformMatrix);
    }

	void RenderableFrustum::InitializeGeometry(const Frustum& frustum)
	{
		InitializeVertexBuffer(frustum);
	}

    void RenderableFrustum::Initialize()
	{
		// Load a compiled vertex shader
		vector<char> compiledVertexShader;
		Utility::LoadBinaryFile(L"Content\\Shaders\\BasicVS.cso", compiledVertexShader);
		ThrowIfFailed(mGame->Direct3DDevice()->CreateVertexShader(&compiledVertexShader[0], compiledVertexShader.size(), nullptr, mVertexShader.ReleaseAndGetAddressOf()), "ID3D11Device::CreatedVertexShader() failed.");

		// Load a compiled pixel shader
		vector<char> compiledPixelShader;
		Utility::LoadBinaryFile(L"Content\\Shaders\\BasicPS.cso", compiledPixelShader);
		ThrowIfFailed(mGame->Direct3DDevice()->CreatePixelShader(&compiledPixelShader[0], compiledPixelShader.size(), nullptr, mPixelShader.ReleaseAndGetAddressOf()), "ID3D11Device::CreatedPixelShader() failed.");

		// Create an input layout
		D3D11_INPUT_ELEMENT_DESC inputElementDescriptions[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		ThrowIfFailed(mGame->Direct3DDevice()->CreateInputLayout(inputElementDescriptions, ARRAYSIZE(inputElementDescriptions), &compiledVertexShader[0], compiledVertexShader.size(), mInputLayout.ReleaseAndGetAddressOf()), "ID3D11Device::CreateInputLayout() failed.");

		// Create constant buffers
		D3D11_BUFFER_DESC constantBufferDesc = { 0 };
		constantBufferDesc.ByteWidth = sizeof(VertexCBufferPerObject);
		constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		ThrowIfFailed(mGame->Direct3DDevice()->CreateBuffer(&constantBufferDesc, nullptr, mVertexCBufferPerObject.ReleaseAndGetAddressOf()), "ID3D11Device::CreateBuffer() failed.");

		InitializeIndexBuffer();
	}

	void RenderableFrustum::Update(const GameTime& gameTime)
	{
		UNREFERENCED_PARAMETER(gameTime);

		XMMATRIX worldMatrix = XMMatrixIdentity();
		MatrixHelper::SetForward(worldMatrix, mDirection);
		MatrixHelper::SetUp(worldMatrix, mUp);
		MatrixHelper::SetRight(worldMatrix, mRight);
		MatrixHelper::SetTranslation(worldMatrix, mPosition);

		XMStoreFloat4x4(&mWorldMatrix, worldMatrix);
	}

	void RenderableFrustum::Draw(const GameTime& gameTime)
	{
		UNREFERENCED_PARAMETER(gameTime);

		ID3D11DeviceContext* direct3DDeviceContext = mGame->Direct3DDeviceContext();
		direct3DDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
		direct3DDeviceContext->IASetInputLayout(mInputLayout.Get());

		uint32_t stride = sizeof(VertexPositionColor);
		uint32_t offset = 0;
		direct3DDeviceContext->IASetVertexBuffers(0, 1, mVertexBuffer.GetAddressOf(), &stride, &offset);
		direct3DDeviceContext->IASetIndexBuffer(mIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);

		direct3DDeviceContext->VSSetShader(mVertexShader.Get(), nullptr, 0);
		direct3DDeviceContext->PSSetShader(mPixelShader.Get(), nullptr, 0);

		XMMATRIX worldMatrix = XMLoadFloat4x4(&mWorldMatrix);
		XMMATRIX wvp = worldMatrix * mCamera->ViewProjectionMatrix();
		XMStoreFloat4x4(&mVertexCBufferPerObjectData.WorldViewProjection, XMMatrixTranspose(wvp));

		direct3DDeviceContext->UpdateSubresource(mVertexCBufferPerObject.Get(), 0, nullptr, &mVertexCBufferPerObjectData, 0, 0);
		direct3DDeviceContext->VSSetConstantBuffers(0, 1, mVertexCBufferPerObject.GetAddressOf());

        direct3DDeviceContext->DrawIndexed(FrustumIndexCount, 0, 0);
	}

	void RenderableFrustum::InitializeVertexBuffer(const Frustum& frustum)
	{
		VertexPositionColor vertices[FrustumVertexCount];
		const XMFLOAT3* corners = frustum.Corners();
		for (uint32_t i = 0; i < FrustumVertexCount; i++)
		{
			vertices[i].Position = XMFLOAT4(corners[i].x, corners[i].y, corners[i].z, 1.0f);
			vertices[i].Color = mColor;
		}

		D3D11_BUFFER_DESC vertexBufferDesc = { 0 };
        vertexBufferDesc.ByteWidth = sizeof(VertexPositionColor) * FrustumVertexCount;
        vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;	
        vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA vertexSubResourceData = { 0 };
        vertexSubResourceData.pSysMem = vertices;
		ThrowIfFailed(mGame->Direct3DDevice()->CreateBuffer(&vertexBufferDesc, &vertexSubResourceData, mVertexBuffer.ReleaseAndGetAddressOf()), "ID3D11Device::CreateBuffer() failed.");
	}

	void RenderableFrustum::InitializeIndexBuffer()
	{
		D3D11_BUFFER_DESC indexBufferDesc = { 0 };
        indexBufferDesc.ByteWidth = sizeof(USHORT) * FrustumIndexCount;
        indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;		
        indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

		D3D11_SUBRESOURCE_DATA indexSubResourceData = { 0 };
        indexSubResourceData.pSysMem = FrustumIndices;
		ThrowIfFailed(mGame->Direct3DDevice()->CreateBuffer(&indexBufferDesc, &indexSubResourceData, mIndexBuffer.ReleaseAndGetAddressOf()), "ID3D11Device::CreateBuffer() failed.");
	}
}