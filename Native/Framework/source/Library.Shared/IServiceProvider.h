#pragma once

#include <cstdint>

namespace Library
{
	class IServiceProvider
	{
	public:
		IServiceProvider() = default;
		IServiceProvider(const IServiceProvider&) = default;
		IServiceProvider& operator=(const IServiceProvider&) = default;
		IServiceProvider(IServiceProvider&&) = default;
		IServiceProvider& operator=(IServiceProvider&&) = default;
		virtual ~IServiceProvider() = default;

		virtual void* GetService(std::uint64_t typeID) const = 0;
	};
}
