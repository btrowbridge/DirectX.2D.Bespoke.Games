#pragma once

#include "Shader.h"
#include <vector>
#include <cstdint>
#include <wrl.h>

namespace Library
{
	class VertexShader final : public Shader
	{
		RTTI_DECLARATIONS(VertexShader, Shader)

	public:
		VertexShader(const VertexShader&) = default;
		VertexShader& operator=(const VertexShader&) = default;
		VertexShader(VertexShader&&) = default;
		VertexShader& operator=(VertexShader&&) = default;
		~VertexShader() = default;

		const std::vector<char>& CompiledShader() const;
		Microsoft::WRL::ComPtr<ID3D11VertexShader> Shader() const;
		Microsoft::WRL::ComPtr<ID3D11InputLayout> InputLayout() const;

		void CreateInputLayout(ID3D11Device* device, const D3D11_INPUT_ELEMENT_DESC* inputElementDescriptions, std::uint32_t inputElementCount, bool releaseCompiledShader = false);

	private:
		friend class VertexShaderReader;

		VertexShader(const std::vector<char>& compiledShader, const Microsoft::WRL::ComPtr<ID3D11VertexShader>& vertexShader);
		VertexShader(std::vector<char>&& compiledShader, const Microsoft::WRL::ComPtr<ID3D11VertexShader>& vertexShader);

		std::vector<char> mCompiledShader;
		Microsoft::WRL::ComPtr<ID3D11VertexShader> mShader;
		Microsoft::WRL::ComPtr<ID3D11InputLayout> mInputLayout;
	};
}
