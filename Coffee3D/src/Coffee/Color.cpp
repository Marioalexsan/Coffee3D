#include <Coffee/Color.hpp>

namespace coffee
{
	const Color Color::White = Color(1, 1, 1);
	const Color Color::Blue = Color(0, 0, 1);
	const Color Color::Red = Color(1, 0, 0);
	const Color Color::Green = Color(0, 1, 0);
	const Color Color::Black = Color(0, 0, 0); 

	Color::Color() :
		r(0),
		g(0),
		b(0),
		a(0)
	{ }

	Color::Color(float r, float g, float b) :
		r(r),
		g(g),
		b(b),
		a(1.0f)
	{ }

	Color::Color(float r, float g, float b, float a) :
		r(r),
		g(g),
		b(b),
		a(a)
	{ }
}