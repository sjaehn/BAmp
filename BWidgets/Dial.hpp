/* Dial.hpp
 * Copyright (C) 2018  Sven Jähnichen
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef BWIDGETS_DIAL_HPP_
#define BWIDGETS_DIAL_HPP_

#include "RangeWidget.hpp"
#include "Knob.hpp"
#include "DrawingSurface.hpp"

#define BWIDGETS_DEFAULT_DIAL_WIDTH 50.0
#define BWIDGETS_DEFAULT_DIAL_HEIGHT 50.0
#define BWIDGETS_DEFAULT_DIAL_KNOB_SIZE 0.6
#define BWIDGETS_DEFAULT_DIAL_DEPTH 1.0
#define BWIDGETS_DEFAULT_DIAL_DOT_SIZE 0.1

namespace BWidgets
{
/**
 * Class BWidgets::Dial
 *
 * RangeWidget class dial.
 * The Widget is clickable by default.
 */
class Dial : public RangeWidget
{
public:
	Dial ();
	Dial (const double x, const double y, const double width, const double height, const std::string& name,
		  const double value, const double min, const double max, const double step);

	/**
	 * Creates a new (orphan) dial and copies the dial properties from a
	 * source dial.
	 * @param that Source dial
	 */
	Dial (const Dial& that);

	~Dial ();

	/**
	 * Assignment. Copies the dial properties from a source dial and keeps
	 * its name and its position within the widget tree. Emits an expose event
	 * if the widget is visible and a value changed event.
	 * @param that Source widget
	 */
	Dial& operator= (const Dial& that);

	/**
	 * Calls a redraw of the widget and calls postRedisplay () if the the
	 * Widget is visible.
	 * This method should be called if the widgets properties are indirectly
	 * changed.
	 */
	virtual void update () override;

	/**
	 * Scans theme for widget properties and applies these properties.
	 * @param theme Theme to be scanned.
	 * 				Styles used are:
	 * 				BWIDGETS_KEYWORD_BORDER
	 * 				BWIDGETS_KEYWORD_BACKGROUND
	 * 				BWIDGETS_KEYWORD_FGCOLORS
	 * 				BWIDGETS_KEYWORD_BGCOLORS
	 * @param name Name of the BStyles::StyleSet within the theme to be
	 * 		  	   applied.
	 */
	virtual void applyTheme (BStyles::Theme& theme);
	virtual void applyTheme (BStyles::Theme& theme, const std::string& name);

	/**
	 * Handles the BEvents::BUTTON_PRESS_EVENT to turn the dial.
	 * @param event Pointer to a poiter event emitted by the same widget.
	 */
	virtual void onButtonPressed (BEvents::PointerEvent* event) override;

	/**
	 * Handles the BEvents::POINTER_MOTION_WHILE_BUTTON_PRESSED_EVENT to turn
	 * the dial.
	 * @param event Pointer to a pointer event emitted by the same widget.
	 */
	virtual void onPointerMotionWhileButtonPressed (BEvents::PointerEvent* event) override;


protected:
	void drawDot ();
	virtual void updateCoords ();
	virtual void draw (const double x, const double y, const double width, const double height) override;

	double dialCenterX;
	double dialCenterY;
	double dialRadius;

	Knob knob;
	DrawingSurface dot;
	BColors::ColorSet fgColors;
	BColors::ColorSet bgColors;
};

}


#endif /* BWIDGETS_DIAL_HPP_ */
