#pragma once

namespace Library
{
	template<typename T>
	constexpr const T& clamp(const T& v, const T& lo, const T& hi)
	{
		return clamp(v, lo, hi, std::less<>());
	}

	template<class T, typename Compare>
	constexpr const T& clamp(const T& v, const T& lo, const T& hi, Compare comp)
	{
		return assert(!comp(hi, lo)), comp(v, lo) ? lo : comp(hi, v) ? hi : v;
	}
}