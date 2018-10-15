#ifndef DRAWINGSURFACE_HPP_
#define DRAWINGSURFACE_HPP_

#include "Widget.hpp"

namespace BWidgets
{
/**
 * Class BWidgets::DrawingSurface
 *
 * Basic button widget drawing a button. Is is a BWidgets::ValueWidget having
 * two conditions: on (value != 0) or off (value == 0). The Widget is clickable
 * by default.
 */
class DrawingSurface : public Widget
{
public:
	DrawingSurface ();
	DrawingSurface (const double x, const double y, const double width, const double height, const std::string& name);
	~DrawingSurface ();

	/**
	 * Gets the colors of the button
	 * @return BColors::ColorSet
	 */
	cairo_surface_t* getDrawingSurface ();

	/**
	 * Calls a redraw of the widget and calls postRedisplay () if the the
	 * Widget is visible.
	 * This method should be called if the widgets properties are indirectly
	 * changed.
	 */
	virtual void update () override;

protected:
	virtual void draw (const double x, const double y, const double width, const double height) override;

	cairo_surface_t* drawingSurface;
};

}

#endif /* DRAWINGSURFACE_HPP_ */
