#ifndef COFFEE_COLOR_HPP
#define COFFEE_COLOR_HPP

namespace coffee
{
	struct Color
	{
		Color();

		Color(float r, float g, float b);

		Color(float r, float g, float b, float a);

		float r;
		float g;
		float b;
		float a;

		static const Color White;
		static const Color Blue;
		static const Color Red;
		static const Color Green;
		static const Color Black;
	};
}

#endif // COFFEE_COLOR_HPP