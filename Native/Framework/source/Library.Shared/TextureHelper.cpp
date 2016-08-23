#include "pch.h"

using namespace DirectX;

namespace Library
{
	Point TextureHelper::GetTextureSize(ID3D11Texture2D* texture)
	{
		D3D11_TEXTURE2D_DESC textureDesc;
		texture->GetDesc(&textureDesc);
		
		return Point(textureDesc.Width, textureDesc.Height);
	}

	Rectangle TextureHelper::GetTextureBounds(ID3D11Texture2D* texture)
	{
		D3D11_TEXTURE2D_DESC textureDesc;
		texture->GetDesc(&textureDesc);

		return Rectangle(0, 0, textureDesc.Width, textureDesc.Height);
	}
}
