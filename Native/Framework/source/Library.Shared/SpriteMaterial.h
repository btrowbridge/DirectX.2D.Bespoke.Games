#pragma once

#include "Material.h"

namespace Library
{
	class Texture2D;

	class SpriteMaterial final : public Material
	{
		RTTI_DECLARATIONS(SpriteMaterial, Material)

	public:
		SpriteMaterial(Game& game, const std::shared_ptr<Texture2D>& texture);
		
		virtual std::uint32_t VertexSize() const override;
		Microsoft::WRL::ComPtr<ID3D11SamplerState> SamplerState() const;
		void SetSamplerState(const Microsoft::WRL::ComPtr<ID3D11SamplerState>& samplerState);		

		virtual void Initialize() override;
		virtual void Draw() override;		

		void UpdateConstantBuffer(DirectX::CXMMATRIX worldViewProjectionMatrix);

	private:
		virtual void BeginDraw() override;

		static const std::uint32_t IndexCount;

		Microsoft::WRL::ComPtr<ID3D11Buffer> mVertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> mIndexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> mConstantBuffer;
		std::shared_ptr<Texture2D> mTexture;
		Microsoft::WRL::ComPtr<ID3D11SamplerState> mSamplerState;
	};
}
