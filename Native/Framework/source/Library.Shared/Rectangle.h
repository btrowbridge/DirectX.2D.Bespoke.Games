#pragma once

#include "Point.h"
#include <windef.h>

namespace Library
{
	class Rectangle
	{
	public:		
		int X;
		int Y;
		int Width;
		int Height;

		Rectangle(const Point& location, const Point& size);		
		Rectangle(int x, int y, int width, int height);

		int Left() const;
		int Right() const;
		int Top() const;
		int Bottom() const;
		Point Size() const;
		void SetSize(const Point& size);
		Point Center() const;
		Point Location() const;
		
		bool Intersects(const Rectangle& other) const;		

		static const Rectangle Empty;
	};
}

#include "Rectangle.inl"