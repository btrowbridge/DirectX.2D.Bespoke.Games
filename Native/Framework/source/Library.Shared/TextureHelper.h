#pragma once

#include <d3d11_2.h>
#include "Rectangle.h"

namespace Library
{
	class TextureHelper final
	{
	public:
		static Point GetTextureSize(ID3D11Texture2D* texture);
		static Rectangle GetTextureBounds(ID3D11Texture2D* texture);
		
		TextureHelper() = delete;
		TextureHelper(const TextureHelper&) = delete;
		TextureHelper& operator=(const TextureHelper&) = delete;
		TextureHelper(TextureHelper&&) = delete;
		TextureHelper& operator=(TextureHelper&&) = delete;
		~TextureHelper() = default;
	};
}