#include "pch.h"
#include "VertexShaderReader.h"

using namespace std;
using namespace DirectX;
using namespace Microsoft::WRL;

namespace Library
{
	RTTI_DEFINITIONS(VertexShaderReader)

	VertexShaderReader::VertexShaderReader(Game& game) :
		ContentTypeReader(game, VertexShader::TypeIdClass())
	{
	}

	shared_ptr<VertexShader> VertexShaderReader::_Read(const wstring& assetName)
	{
		ComPtr<ID3D11VertexShader> vertexShader;
		vector<char> compiledVertexShader;
		Utility::LoadBinaryFile(assetName, compiledVertexShader);
		ThrowIfFailed(mGame->Direct3DDevice()->CreateVertexShader(&compiledVertexShader[0], compiledVertexShader.size(), nullptr, vertexShader.ReleaseAndGetAddressOf()), "ID3D11Device::CreatedVertexShader() failed.");
		
		return shared_ptr<VertexShader>(new VertexShader(move(compiledVertexShader), vertexShader));
	}
}