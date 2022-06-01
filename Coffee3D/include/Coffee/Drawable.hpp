#ifndef COFFEE_DRAWABLE_HPP
#define COFFEE_DRAWABLE_HPP

#include <Coffee/RenderState.hpp>

namespace cf
{
	class Drawable
	{
	public:
		virtual ~Drawable() {}
		virtual void draw(RenderState state) const = 0;
	};
}

#endif