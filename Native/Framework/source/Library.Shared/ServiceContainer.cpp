#include "pch.h"
#include "ServiceContainer.h"

using namespace std;

namespace Library
{
	bool ServiceContainer::AddService(uint64_t typeID, void* service)
	{
		return mServices.emplace(typeID, service).second;
	}

	void ServiceContainer::RemoveService(uint64_t typeID)
	{
		mServices.erase(typeID);
	}

	void* ServiceContainer::GetService(uint64_t typeID) const
	{
		map<uint64_t, void*>::const_iterator it = mServices.find(typeID);

		return (it != mServices.cend() ? it->second : nullptr);
	}
}
