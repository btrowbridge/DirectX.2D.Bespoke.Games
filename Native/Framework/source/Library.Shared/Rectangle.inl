#include "Rectangle.h"

namespace Library
{	
	inline int Rectangle::Left() const
	{
		return X;
	}

	inline int Rectangle::Right() const
	{
		return X + Width;
	}

	inline int Rectangle::Top() const
	{
		return Y;
	}

	inline int Rectangle::Bottom() const
	{
		return Y + Height;
	}

	inline Point Rectangle::Size() const
	{
		return Point(Width, Height);
	}

	inline void Rectangle::SetSize(const Point& size)
	{
		Width = size.X;
		Height = size.Y;
	}

	inline Point Rectangle::Center() const
	{
		return Point(X + (Width / 2), Y + (Height / 2));
	}

	inline Point Rectangle::Location() const
	{
		return Point(X, Y);
	}
}