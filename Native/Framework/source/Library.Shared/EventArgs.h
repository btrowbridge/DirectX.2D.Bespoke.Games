#pragma once

#include "RTTI.h"

namespace Library
{
	class EventArgs : public RTTI
	{
		RTTI_DECLARATIONS(EventArgs, RTTI)

	public:
		static const EventArgs Empty;
	};
}