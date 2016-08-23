#pragma once

#include "IServiceProvider.h"
#include <map>

namespace Library
{
	class ServiceContainer final : public IServiceProvider
	{
	public:
		ServiceContainer() = default;
		ServiceContainer(const ServiceContainer&) = delete;
		ServiceContainer& operator=(const ServiceContainer&) = delete;

		void AddService(std::uint64_t typeID, void* service);
		void RemoveService(std::uint64_t typeID);
		virtual void* GetService(std::uint64_t typeID) const override;;

		std::map<std::uint64_t, void*> mServices;
	};
}