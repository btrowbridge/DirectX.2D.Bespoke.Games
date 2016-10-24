#pragma once

#include "Texture.h"
#include "Rectangle.h"
#include <cstdint>

namespace Library
{
	class Texture2D final : public Texture
	{
		RTTI_DECLARATIONS(Texture2D, Texture)

	public:
		Texture2D(const Texture2D&) = default;
		Texture2D& operator=(const Texture2D&) = default;
		Texture2D(Texture2D&&) = default;
		Texture2D& operator=(Texture2D&&) = default;
		~Texture2D() = default;

		std::int32_t Width() const;
		std::int32_t Height() const;
		Rectangle Bounds() const;

	private:
		friend class Texture2DReader;

		Texture2D(const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& shaderResourceView, const std::int32_t width, const std::int32_t height);

		std::int32_t mWidth;
		std::int32_t mHeight;
	};
}
