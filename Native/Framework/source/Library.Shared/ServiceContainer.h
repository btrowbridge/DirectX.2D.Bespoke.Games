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
		ServiceContainer(ServiceContainer&&) = delete;
		ServiceContainer& operator=(ServiceContainer&&) = delete;

		bool AddService(std::uint64_t typeID, void* service);
		void RemoveService(std::uint64_t typeID);
		virtual void* GetService(std::uint64_t typeID) const override;;

	private:
		std::map<std::uint64_t, void*> mServices;
	};
}