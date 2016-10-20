#pragma once

#include <Material.h>

namespace SimpleCollision
{
	class PositionColorMaterial final : public Library::Material
	{
		RTTI_DECLARATIONS(PositionColorMaterial, Library::Material)

	public:
		PositionColorMaterial(Library::Game& game);

		virtual void Initialize() override;
		virtual std::uint32_t VertexSize() const override;

	private:
		struct VSCBufferPerObject
		{
			DirectX::XMFLOAT4X4 WorldViewProjection;

			VSCBufferPerObject() = default;
			VSCBufferPerObject(const DirectX::XMFLOAT4X4& wvp) :
				WorldViewProjection(wvp) { }
		};

		virtual void BeginDraw() override;

		DirectX::XMFLOAT4X4 mWorldMatrix;
		Microsoft::WRL::ComPtr<ID3D11Buffer> mVSCBufferPerObject;
		VSCBufferPerObject mVSCBufferPerObjectData;
	};
}