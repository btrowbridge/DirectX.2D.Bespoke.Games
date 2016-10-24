#pragma once

#include "DrawableGameComponent.h"
#include <d3d11_2.h>
#include <DirectXMath.h>
#include <wrl.h>

namespace Library
{
	class Frustum;

    class RenderableFrustum final : public DrawableGameComponent
    {
        RTTI_DECLARATIONS(RenderableFrustum, DrawableGameComponent)

    public:
        RenderableFrustum(Game& game, const std::shared_ptr<Camera>& camera, const DirectX::XMFLOAT4& color = DefaultColor);
		RenderableFrustum(const RenderableFrustum&) = delete;
		RenderableFrustum(RenderableFrustum&&) = delete;
		RenderableFrustum& operator=(const RenderableFrustum&) = delete;
		RenderableFrustum& operator=(RenderableFrustum&&) = delete;

		const DirectX::XMFLOAT3& Position() const;
        const DirectX::XMFLOAT3& Direction() const;
        const DirectX::XMFLOAT3& Up() const;
        const DirectX::XMFLOAT3& Right() const;

		DirectX::XMVECTOR PositionVector() const;
		DirectX::XMVECTOR DirectionVector() const;
		DirectX::XMVECTOR UpVector() const;
		DirectX::XMVECTOR RightVector() const;

		void SetPosition(float x, float y, float z);
		void SetPosition(DirectX::FXMVECTOR position);
        void SetPosition(const DirectX::XMFLOAT3& position);

		void ApplyRotation(DirectX::CXMMATRIX transform);
        void ApplyRotation(const DirectX::XMFLOAT4X4& transform);

		void InitializeGeometry(const Frustum& frustum);

        virtual void Initialize() override;
        virtual void Update(const GameTime& gameTime) override;
		virtual void Draw(const GameTime& gameTime) override;

    private:
		struct VertexCBufferPerObject
		{
			DirectX::XMFLOAT4X4 WorldViewProjection;

			VertexCBufferPerObject() : WorldViewProjection() { }

			VertexCBufferPerObject(const DirectX::XMFLOAT4X4& wvp) : WorldViewProjection(wvp) { }
		};

		void InitializeVertexBuffer(const Frustum& frustum);
		void InitializeIndexBuffer();

		static const DirectX::XMFLOAT4 DefaultColor;
		static const std::uint32_t FrustumVertexCount;		
		static const std::uint32_t FrustumPrimitiveCount;
		static const std::uint32_t FrustumIndicesPerPrimitive;
		static const std::uint32_t FrustumIndexCount;
		static const std::uint16_t FrustumIndices[];

		Microsoft::WRL::ComPtr<ID3D11VertexShader> mVertexShader;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> mPixelShader;
		Microsoft::WRL::ComPtr<ID3D11InputLayout> mInputLayout;
		Microsoft::WRL::ComPtr<ID3D11Buffer> mVertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> mIndexBuffer;		
		Microsoft::WRL::ComPtr<ID3D11Buffer> mVertexCBufferPerObject;
		VertexCBufferPerObject mVertexCBufferPerObjectData;
		
		DirectX::XMFLOAT4 mColor;
		DirectX::XMFLOAT3 mPosition;
		DirectX::XMFLOAT3 mDirection;
        DirectX::XMFLOAT3 mUp;
        DirectX::XMFLOAT3 mRight;

		DirectX::XMFLOAT4X4 mWorldMatrix;
    };
}