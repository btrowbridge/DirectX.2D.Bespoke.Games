#pragma once

#include <cstdint>

namespace Library
{
	class Point final
	{
	public:
		std::int32_t X;
		std::int32_t Y;

		explicit Point(const std::int32_t x = 0, const std::int32_t y = 0);

		bool operator<(const Point& rhs) const;

		static const Point Zero;
	};
}

#include "Point.inl"