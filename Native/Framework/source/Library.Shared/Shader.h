#pragma once

#include "RTTI.h"
#include <d3d11_2.h>

namespace Library
{
	class Shader abstract : public RTTI
	{
		RTTI_DECLARATIONS(Shader, RTTI)

	public:		
		Shader(const Shader&) = delete;
		Shader& operator=(const Shader&) = delete;
		Shader(Shader&&) = delete;
		Shader& operator=(Shader&&) = delete;
		virtual ~Shader() = default;

	protected:
		Shader() = default;
	};
}