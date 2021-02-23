#ifndef GALGUI_CANVAS
#define GALGUI_CANVAS

#include <internal/circle.hpp>
#include <internal/color.hpp>
#include <include/gal_support.hpp>

namespace gal::internal
{
	class basic_canvas : gal::non_copyable
	{
	public:
		///////////////////////////////////////////////////////////////////////////////////
		// Transforms
		void translate(const point&);
		void rotate(float rad);
		void scale(const extent&);

	};
}

#endif
