#include "DrawingSurface.hpp"

namespace BWidgets
{
DrawingSurface::DrawingSurface () : DrawingSurface (0.0, 0.0, 50.0, 50.0, "dial") {}

DrawingSurface::DrawingSurface (const double x, const double y, const double width, const double height, const std::string& name) :
		Widget (x, y, width, height, name)
{
	drawingSurface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, width, height);
	draw (0, 0, width_, height_);
}

DrawingSurface:: ~DrawingSurface ()
{
	cairo_surface_destroy (drawingSurface);
}

cairo_surface_t* DrawingSurface::getDrawingSurface () {return drawingSurface;}

void DrawingSurface::update ()
{
	draw (0, 0, width_, height_);
	if (isVisible ()) postRedisplay ();
}

void DrawingSurface::draw (const double x, const double y, const double width, const double height)
{
	if ((width_ >= 4) && (height_ >= 4))
	{
		// Draw super class widget elements first
		Widget::draw (x, y, width, height);

		cairo_t* cr = cairo_create (widgetSurface);
		if (cairo_status (cr) == CAIRO_STATUS_SUCCESS)
		{
			// Limit cairo-drawing area
			cairo_rectangle (cr, x, y, width, height);
			cairo_clip (cr);

			cairo_set_source_surface (cr, drawingSurface, 0, 0);
			cairo_paint (cr);
		}
		cairo_destroy (cr);
	}
}

}
