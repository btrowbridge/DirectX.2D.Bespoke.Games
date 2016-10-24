#pragma once

#include "RTTI.h"
#include <wrl.h>
#include <d3d11_2.h>

namespace Library
{
	class Texture abstract : public RTTI
	{
		RTTI_DECLARATIONS(Texture, RTTI)

	public:		
		Texture(const Texture&) = delete;
		Texture& operator=(const Texture&) = delete;
		Texture(Texture&&) = delete;
		Texture& operator=(Texture&&) = delete;
		virtual ~Texture() = default;

		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> ShaderResourceView() const;

	protected:
		Texture(const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& shaderResourceView);

		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mShaderResourceView;
	};
}

#include "Texture.inl"