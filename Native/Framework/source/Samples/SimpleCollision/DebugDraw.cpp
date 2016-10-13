#include "pch.h"
#include "DebugDraw.h"

using namespace std;
using namespace DirectX;
using namespace Library;
using namespace Microsoft::WRL;

namespace SimpleCollision {
	DebugDraw::DebugDraw(Library::Game& game, std::shared_ptr<Library::Camera> camera) :
		DrawableGameComponent(game, camera), b2Draw(), mCamera(camera), mWorldMatrix(MatrixHelper::Identity), mPixelShader(), mVertexShader()
	{
	}

	DebugDraw::~DebugDraw()
	{
	}
	void DebugDraw::DrawPolygon(const b2Vec2 * vertices, int32 vertexCount, const b2Color & color)
	{
		std::vector<VertexPositionColor> vpc;

		XMFLOAT4 vertexColor(color.r, color.g, color.b, color.a);

		for (int32 i = 0; i < vertexCount; i++) {
			XMFLOAT4 position(vertices[i].x, vertices[i].y, 0.0f, 1.0f);
			vpc.push_back(VertexPositionColor(position, vertexColor));
		}
		//line strip
		vpc.push_back(VertexPositionColor(XMFLOAT4(vertices[0].x, vertices[0].y, 0.0f, 1.0f), vertexColor));
		vertexCount++;

		ID3D11DeviceContext* direct3DDeviceContext = mGame->Direct3DDeviceContext();
		
		D3D11_MAPPED_SUBRESOURCE mappedSubResource;
		direct3DDeviceContext->Map(mPolygonVertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, NULL, &mappedSubResource);
		memcpy(mappedSubResource.pData, &vpc[0], sizeof(VertexPositionColor) * vertexCount);
		direct3DDeviceContext->Unmap(mPolygonVertexBuffer.Get(), 0);
		
		direct3DDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

		direct3DDeviceContext->IASetInputLayout(mInputLayout.Get());

		UINT stride = sizeof(VertexPositionColor);
		UINT offset = 0;

		direct3DDeviceContext->IASetVertexBuffers(0, 1, mPolygonVertexBuffer.GetAddressOf(), &stride, &offset);

		direct3DDeviceContext->VSSetShader(mVertexShader.Get(), nullptr, 0);
		direct3DDeviceContext->PSSetShader(mPixelShader.Get(), nullptr, 0);

		direct3DDeviceContext->UpdateSubresource(mConstantBuffer.Get(), 0, nullptr, &mCBufferPerObject, 0, 0);
		direct3DDeviceContext->VSSetConstantBuffers(0, 1, mConstantBuffer.GetAddressOf());

		direct3DDeviceContext->Draw(vertexCount, 0);
	}
	void DebugDraw::DrawSolidPolygon(const b2Vec2 * vertices, int32 vertexCount, const b2Color & color)
	{
		std::vector<VertexPositionColor> vpc;

		XMFLOAT4 vertexColor(color.r, color.g, color.b, color.a);

		for (int32 i = 0; i < vertexCount; i++) {
			XMFLOAT4 position(vertices[i].x, vertices[i].y, 0.0f, 1.0f);
			vpc.push_back(VertexPositionColor(position, vertexColor));
		}
		//line strip
		vpc.push_back(VertexPositionColor(XMFLOAT4(vertices[0].x, vertices[0].y, 0.0f, 1.0f), vertexColor));
		vertexCount++;

		ID3D11DeviceContext* direct3DDeviceContext = mGame->Direct3DDeviceContext();

		D3D11_MAPPED_SUBRESOURCE mappedSubResource;
		direct3DDeviceContext->Map(mPolygonVertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, NULL, &mappedSubResource);
		memcpy(mappedSubResource.pData, &vpc[0], sizeof(VertexPositionColor) * vertexCount);
		direct3DDeviceContext->Unmap(mPolygonVertexBuffer.Get(), 0);
		
		direct3DDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

		direct3DDeviceContext->IASetInputLayout(mInputLayout.Get());

		UINT stride = sizeof(VertexPositionColor);
		UINT offset = 0;

		direct3DDeviceContext->IASetVertexBuffers(0, 1, mPolygonVertexBuffer.GetAddressOf(), &stride, &offset);

		direct3DDeviceContext->VSSetShader(mVertexShader.Get(), nullptr, 0);
		direct3DDeviceContext->PSSetShader(mPixelShader.Get(), nullptr, 0);

		direct3DDeviceContext->UpdateSubresource(mConstantBuffer.Get(), 0, nullptr, &mCBufferPerObject, 0, 0);
		direct3DDeviceContext->VSSetConstantBuffers(0, 1, mConstantBuffer.GetAddressOf());

		direct3DDeviceContext->Draw(vertexCount, 0);
	}
	void DebugDraw::DrawCircle(const b2Vec2 & center, float32 radius, const b2Color & color)
	{
		std::vector<VertexPositionColor> vpc;
		const float32 k_segments = 16.0f;
		const float32 k_increment = 2.0f * b2_pi / k_segments;
		float32 sinInc = sinf(k_increment);
		float32 cosInc = cosf(k_increment);
		b2Vec2 r1(1.0f, 0.0f);
		b2Vec2 v1 = center + radius * r1;

		XMFLOAT4 vertexColor(color.r, color.b, color.g, color.a);

		VertexPositionColor end(XMFLOAT4(v1.x, v1.y, 0.0f, 1.0f), vertexColor);

		vpc.push_back(VertexPositionColor(XMFLOAT4(center.x,center.y,0.0f,1.0f), vertexColor));

		int vertexCount = 1;
		for (int32 i = 0; i < k_segments; ++i)
		{
			// Perform rotation to avoid additional trigonometry.
			b2Vec2 r2;
			r2.x = cosInc * r1.x - sinInc * r1.y;
			r2.y = sinInc * r1.x + cosInc * r1.y;
			b2Vec2 v2 = center + radius * r2;
			vpc.push_back(VertexPositionColor(XMFLOAT4(v2.x,v2.y,0.0f,1.0f), vertexColor));
			vertexCount++;
			r1 = r2;
			v1 = v2;
		}
		vpc.push_back(end);
		vertexCount++;

		ID3D11DeviceContext* direct3DDeviceContext = mGame->Direct3DDeviceContext();

		D3D11_MAPPED_SUBRESOURCE mappedSubResource;
		direct3DDeviceContext->Map(mCircleVertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, NULL, &mappedSubResource);
		memcpy(mappedSubResource.pData, &vpc[0], sizeof(VertexPositionColor) * vertexCount);
		direct3DDeviceContext->Unmap(mCircleVertexBuffer.Get(), 0);
		
		direct3DDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

		direct3DDeviceContext->IASetInputLayout(mInputLayout.Get());

		UINT stride = sizeof(VertexPositionColor);
		UINT offset = 0;

		direct3DDeviceContext->IASetVertexBuffers(0, 1, mCircleVertexBuffer.GetAddressOf(), &stride, &offset);

		direct3DDeviceContext->VSSetShader(mVertexShader.Get(), nullptr, 0);
		direct3DDeviceContext->PSSetShader(mPixelShader.Get(), nullptr, 0);


		direct3DDeviceContext->UpdateSubresource(mConstantBuffer.Get(), 0, nullptr, &mCBufferPerObject, 0, 0);
		direct3DDeviceContext->VSSetConstantBuffers(0, 1, mConstantBuffer.GetAddressOf());

		direct3DDeviceContext->Draw(vertexCount, 0);

	}
	void DebugDraw::DrawSolidCircle(const b2Vec2 & center, float32 radius, const b2Vec2 & axis, const b2Color & color)
	{
	
		std::vector<VertexPositionColor> vpc;
		const float32 k_segments = 16.0f;
		const float32 k_increment = 2.0f * b2_pi / k_segments;
		float32 sinInc = sinf(k_increment);
		float32 cosInc = cosf(k_increment);
		b2Vec2 r1 = axis;
		b2Vec2 v1 = center + radius * r1;

		XMFLOAT4 vertexColor(color.r, color.b, color.g, color.a);

		VertexPositionColor end(XMFLOAT4(v1.x, v1.y, 0.0f, 1.0f), vertexColor);

		vpc.push_back(VertexPositionColor(XMFLOAT4(center.x,center.y,0.0f,1.0f), vertexColor));

		int vertexCount = 1;
		for (int32 i = 0; i < k_segments; ++i)
		{
			// Perform rotation to avoid additional trigonometry.
			b2Vec2 r2;
			r2.x = cosInc * r1.x - sinInc * r1.y;
			r2.y = sinInc * r1.x + cosInc * r1.y;
			b2Vec2 v2 = center + radius * r2;
			vpc.push_back(VertexPositionColor(XMFLOAT4(v2.x,v2.y,0.0f,1.0f), vertexColor));
			vertexCount++;
			r1 = r2;
			v1 = v2;
		}
		vpc.push_back(end);
		vertexCount++;
		

		ID3D11DeviceContext* direct3DDeviceContext = mGame->Direct3DDeviceContext();

		D3D11_MAPPED_SUBRESOURCE mappedSubResource;
		direct3DDeviceContext->Map(mCircleVertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, NULL, &mappedSubResource);
		memcpy(mappedSubResource.pData, &vpc[0], sizeof(VertexPositionColor) * vertexCount);
		direct3DDeviceContext->Unmap(mCircleVertexBuffer.Get(), 0);
		
		direct3DDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

		direct3DDeviceContext->IASetInputLayout(mInputLayout.Get());

		UINT stride = sizeof(VertexPositionColor);
		UINT offset = 0;

		direct3DDeviceContext->IASetVertexBuffers(0, 1, mCircleVertexBuffer.GetAddressOf(), &stride, &offset);

		direct3DDeviceContext->VSSetShader(mVertexShader.Get(), nullptr, 0);
		direct3DDeviceContext->PSSetShader(mPixelShader.Get(), nullptr, 0);


		direct3DDeviceContext->UpdateSubresource(mConstantBuffer.Get(), 0, nullptr, &mCBufferPerObject, 0, 0);
		direct3DDeviceContext->VSSetConstantBuffers(0, 1, mConstantBuffer.GetAddressOf());

		direct3DDeviceContext->Draw(vertexCount, 0);

	}
	void DebugDraw::DrawSegment(const b2Vec2 & p1, const b2Vec2 & p2, const b2Color & color)
	{
		std::vector<VertexPositionColor> vpc;

		XMFLOAT4 vertexColor(color.r, color.g, color.b, color.a);
		
		vpc.push_back(VertexPositionColor(XMFLOAT4(p1.x, p1.y, 0.0f, 1.0f), vertexColor));
		vpc.push_back(VertexPositionColor(XMFLOAT4(p2.x, p2.y, 0.0f, 1.0f), vertexColor));
		int vertexCount = 2;

		ID3D11DeviceContext* direct3DDeviceContext = mGame->Direct3DDeviceContext();

		D3D11_MAPPED_SUBRESOURCE mappedSubResource;
		direct3DDeviceContext->Map(mPolygonVertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, NULL, &mappedSubResource);
		memcpy(mappedSubResource.pData, &vpc[0], sizeof(VertexPositionColor) * vertexCount);
		direct3DDeviceContext->Unmap(mPolygonVertexBuffer.Get(), 0);
		
		direct3DDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

		direct3DDeviceContext->IASetInputLayout(mInputLayout.Get());

		UINT stride = sizeof(VertexPositionColor);
		UINT offset = 0;

		direct3DDeviceContext->IASetVertexBuffers(0, 1, mPolygonVertexBuffer.GetAddressOf(), &stride, &offset);

		direct3DDeviceContext->VSSetShader(mVertexShader.Get(), nullptr, 0);
		direct3DDeviceContext->PSSetShader(mPixelShader.Get(), nullptr, 0);

		direct3DDeviceContext->UpdateSubresource(mConstantBuffer.Get(), 0, nullptr, &mCBufferPerObject, 0, 0);
		direct3DDeviceContext->VSSetConstantBuffers(0, 1, mConstantBuffer.GetAddressOf());

		direct3DDeviceContext->Draw(vertexCount, 0);

	}
	void DebugDraw::DrawTransform(const b2Transform & xf)
	{
		UNREFERENCED_PARAMETER(xf);
	}
	void DebugDraw::DrawPoint(const b2Vec2 & p, float32 size, const b2Color & color)
	{
		UNREFERENCED_PARAMETER(p);
		UNREFERENCED_PARAMETER(size);
		UNREFERENCED_PARAMETER(color);
	}

	void DebugDraw::Initialize()
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

		ThrowIfFailed(mGame->Direct3DDevice()->CreateInputLayout(inputElementDescriptions,
			ARRAYSIZE(inputElementDescriptions), &compiledVertexShader[0], compiledVertexShader.size(),
			mInputLayout.ReleaseAndGetAddressOf()), "ID3D11Device::CreateInputLayout() failed.");

		D3D11_BUFFER_DESC constantBufferDesc = { 0 };
		constantBufferDesc.ByteWidth = sizeof(CBufferPerObject);
		constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		ThrowIfFailed(mGame->Direct3DDevice()->CreateBuffer(&constantBufferDesc, nullptr, mConstantBuffer.ReleaseAndGetAddressOf()), "ID3D11Device::CreateBuffer() failed.");

		//polygon buffer
		D3D11_BUFFER_DESC vertexBufferDesc = { 0 };
		vertexBufferDesc.ByteWidth = sizeof(VertexPositionColor) * b2_maxPolygonVertices;
		vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		ThrowIfFailed(mGame->Direct3DDevice()->CreateBuffer(
			&vertexBufferDesc, nullptr, mPolygonVertexBuffer.ReleaseAndGetAddressOf()), "ID3D11Device::CreateBuffer() failed.");
	
		//circle buffer
		vertexBufferDesc.ByteWidth = sizeof(VertexPositionColor) * UINT(18);

		ThrowIfFailed(mGame->Direct3DDevice()->CreateBuffer(
			&vertexBufferDesc, nullptr, mCircleVertexBuffer.ReleaseAndGetAddressOf()), "ID3D11Device::CreateBuffer() failed.");

	}

	void DebugDraw::Update(const Library::GameTime & gameTime)
	{
		UNREFERENCED_PARAMETER(gameTime);
	}

	void DebugDraw::Draw(const Library::GameTime& gameTime)
	{
		UNREFERENCED_PARAMETER(gameTime);
		XMMATRIX worldMatrix = XMLoadFloat4x4(&mWorldMatrix);
		XMMATRIX wvp = worldMatrix * mCamera->ViewProjectionMatrix();
		wvp = XMMatrixTranspose(wvp);
		XMStoreFloat4x4(&mCBufferPerObject.WorldViewProjection, wvp);
	}
}