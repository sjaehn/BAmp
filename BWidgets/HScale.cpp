/* HScale.cpp
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

#include "HScale.hpp"

namespace BWidgets
{
HScale::HScale () : HScale (0.0, 0.0, BWIDGETS_DEFAULT_HSCALE_WIDTH, BWIDGETS_DEFAULT_HSCALE_HEIGHT, "hscale",
		  	  	  	  	  	   BWIDGETS_DEFAULT_VALUE, BWIDGETS_DEFAULT_RANGE_MIN, BWIDGETS_DEFAULT_RANGE_MAX, BWIDGETS_DEFAULT_RANGE_STEP) {}

HScale::HScale (const double  x, const double y, const double width, const double height, const std::string& name,
				  const double value, const double min, const double max, const double step) :
		RangeWidget (x, y, width, height, name, value, min, max, step),
		fgColors (BWIDGETS_DEFAULT_FGCOLORS), bgColors (BWIDGETS_DEFAULT_BGCOLORS),
		scaleX0 (0), scaleY0 (0), scaleWidth (width), scaleHeight (height), scaleXValue (0)
{
	setClickable (true);
	setDragable (true);
}

HScale::HScale (const HScale& that) :
		RangeWidget (that), fgColors (that.fgColors), bgColors (that.bgColors), scaleX0 (that.scaleX0), scaleY0 (that.scaleY0),
		scaleWidth (that.scaleWidth), scaleHeight (that.scaleHeight), scaleXValue (that.scaleXValue)
{}

HScale::~HScale () {}

HScale& HScale::operator= (const HScale& that)
{
	fgColors = that.fgColors;
	bgColors = that.bgColors;
	scaleX0 = that.scaleX0;
	scaleY0 = that.scaleY0;
	scaleWidth = that.scaleWidth;
	scaleHeight = that.scaleHeight;
	scaleXValue = that.scaleXValue;
	RangeWidget::operator= (that);

	return *this;
}

void HScale::update ()
{
	updateCoords ();
	Widget::update();
}

void HScale::applyTheme (BStyles::Theme& theme) {applyTheme (theme, name_);}

void HScale::applyTheme (BStyles::Theme& theme, const std::string& name)
{
	Widget::applyTheme (theme, name);

	// Foreground colors (scale)
	void* fgPtr = theme.getStyle(name, BWIDGETS_KEYWORD_FGCOLORS);
	if (fgPtr) fgColors = *((BColors::ColorSet*) fgPtr);

	// Background colors (scale background, knob)
	void* bgPtr = theme.getStyle(name, BWIDGETS_KEYWORD_BGCOLORS);
	if (bgPtr) bgColors = *((BColors::ColorSet*) bgPtr);

	if (fgPtr || bgPtr) update ();

}

void HScale::onButtonPressed (BEvents::PointerEvent* event)
{
	if (main_ && isVisible () && (height_ >= 1) && (width_ >= 1) && (event->getButton() == BEvents::LEFT_BUTTON))
	{
		// Get pointer coords
		double y = event->getY ();
		double x = event->getX ();

		// Pointer within the scale area ? Set value!
		if ((scaleWidth > 0) && (x >= scaleX0) && (x <= scaleX0 + scaleWidth) && (y >= scaleY0 - scaleHeight) && (y <= scaleY0 + 2 * scaleHeight))
		{
			double frac = (x - scaleX0) / scaleWidth;
			if (getStep () < 0) frac = 1 - frac;

			double min = getMin ();
			double max = getMax ();
			setValue (min + frac * (max - min));
		}
	}
}

void HScale::onPointerMotionWhileButtonPressed (BEvents::PointerEvent* event) {onButtonPressed (event);}

void HScale::updateCoords ()
{
	scaleX0 = getXOffset ();
	scaleY0 = getYOffset ();
	scaleWidth = getEffectiveWidth ();
	scaleHeight = getEffectiveHeight ();
	scaleXValue = scaleX0 + getRelativeValue () * scaleWidth;
}

void HScale::draw (const double x, const double y, const double width, const double height)
{
	if ((!widgetSurface) || (cairo_surface_status (widgetSurface) != CAIRO_STATUS_SUCCESS)) return;

	// Draw super class widget elements first
	Widget::draw (x, y, width, height);

	// Draw scale only if it is not a null widget
	if ((scaleHeight >= 1) && (scaleWidth >= 1))
	{
		cairo_surface_clear (widgetSurface);
		cairo_t* cr = cairo_create (widgetSurface);

		if (cairo_status (cr) == CAIRO_STATUS_SUCCESS)
		{
			cairo_pattern_t* pat;

			// Limit cairo-drawing area
			cairo_rectangle (cr, x, y, width, height);
			cairo_clip (cr);

			// Calculate aspect ratios first
			double h = scaleHeight;
			double w = scaleWidth;
			double x1 = scaleX0; double y1 = scaleY0;				// Top left
			double x2 = scaleXValue; double y2 = y1 + h; 			// Value line bottom
			double x3 = scaleXValue; double y3 = y1;				// Value line top
			double x4 = x1 + w; double y4 = y2; 					// Bottom right

			// Colors uses within this method
			BColors::Color fgHi = *fgColors.getColor (getState ()); fgHi.applyBrightness (BWIDGETS_DEFAULT_ILLUMINATED);
			BColors::Color fgMid = *fgColors.getColor (getState ()); fgMid.applyBrightness ((BWIDGETS_DEFAULT_ILLUMINATED + BWIDGETS_DEFAULT_NORMALLIGHTED) / 2);
			BColors::Color fgLo = *fgColors.getColor (getState ()); fgLo.applyBrightness (BWIDGETS_DEFAULT_NORMALLIGHTED);
			BColors::Color bgLo = *bgColors.getColor (getState ()); bgLo.applyBrightness (BWIDGETS_DEFAULT_NORMALLIGHTED);
			BColors::Color bgHi = *bgColors.getColor (getState ()); bgHi.applyBrightness (BWIDGETS_DEFAULT_ILLUMINATED);
			BColors::Color bgMid = *bgColors.getColor (getState ()); bgMid.applyBrightness ((BWIDGETS_DEFAULT_ILLUMINATED + BWIDGETS_DEFAULT_NORMALLIGHTED) / 2);
			BColors::Color bgSh = *bgColors.getColor (getState ()); bgSh.applyBrightness (BWIDGETS_DEFAULT_SHADOWED);

			cairo_set_line_width (cr, 0.0);
			cairo_rectangle_rounded (cr, x1, y1, x4 - x1, y4 - y1, (y4 - y1) / 2);
			cairo_clip (cr);

			// Frame background
			pat = cairo_pattern_create_linear (x4, y4, x1, y1);
			if (pat && (cairo_pattern_status (pat) == CAIRO_STATUS_SUCCESS))
			{
				cairo_pattern_add_color_stop_rgba (pat, 0, bgLo.getRed (), bgLo.getGreen (), bgLo.getBlue (), bgLo.getAlpha ());
				cairo_pattern_add_color_stop_rgba (pat, 1, bgHi.getRed (), bgHi.getGreen (), bgHi.getBlue (), bgHi.getAlpha ());
				cairo_rectangle_rounded (cr, x1, y1, x4 - x1, y4 - y1, (y4 - y1) / 2);
				cairo_set_source (cr, pat);
				cairo_fill (cr);
				cairo_pattern_destroy (pat);
			}
			cairo_rectangle_rounded (cr, x1 + BWIDGETS_DEFAULT_HSCALE_DEPTH, y1 + BWIDGETS_DEFAULT_HSCALE_DEPTH, x4 - x1, y4 - y1, (y4 - y1) / 2);
			cairo_set_source_rgba (cr, bgSh.getRed (), bgSh.getGreen (), bgSh.getBlue (), bgSh.getAlpha ());
			cairo_fill (cr);

			// Scale active
			pat = cairo_pattern_create_linear (x3, y3, x2, y2);
			if (pat && (cairo_pattern_status (pat) == CAIRO_STATUS_SUCCESS))
			{
				cairo_pattern_add_color_stop_rgba (pat, 1, fgLo.getRed (), fgLo.getGreen (), fgLo.getBlue (), fgLo.getAlpha ());
				cairo_pattern_add_color_stop_rgba (pat, 0.75, fgHi.getRed (), fgHi.getGreen (), fgHi.getBlue (), fgHi.getAlpha ());
				cairo_pattern_add_color_stop_rgba (pat, 0, fgLo.getRed (), fgLo.getGreen (), fgLo.getBlue (), fgLo.getAlpha ());
				if (getStep () < 0)
				{
					cairo_rectangle_rounded (cr, x3 + 0.5 * BWIDGETS_DEFAULT_HSCALE_DEPTH,
											 y3 + 0.5 * BWIDGETS_DEFAULT_HSCALE_DEPTH, x4 - x3, y4 - y3, (y4 - y3) / 2, 0b0110);
				}
				else
				{
					cairo_rectangle_rounded (cr, x1 + 0.5 * BWIDGETS_DEFAULT_HSCALE_DEPTH,
											 y1 + 0.5 * BWIDGETS_DEFAULT_HSCALE_DEPTH, x2 - x1, y2 - y1, (y2 - y1) / 2, 0b1001);
				}
				cairo_set_source (cr, pat);
				cairo_fill (cr);
				cairo_pattern_destroy (pat);
			}

			//Frame
			pat = cairo_pattern_create_linear (x4, y4, x1, y1);
			if (pat && (cairo_pattern_status (pat) == CAIRO_STATUS_SUCCESS))
			{
				cairo_pattern_add_color_stop_rgba (pat, 0, bgLo.getRed (), bgLo.getGreen (), bgLo.getBlue (), bgLo.getAlpha ());
				cairo_pattern_add_color_stop_rgba (pat, 1, bgHi.getRed (), bgHi.getGreen (), bgHi.getBlue (), bgHi.getAlpha ());
				cairo_rectangle_rounded (cr, x1, y1, x4 - x1, y4 - y1, (y4 - y1) / 2);
				cairo_set_source (cr, pat);
				cairo_set_line_width (cr, 0.2 * BWIDGETS_DEFAULT_HSCALE_DEPTH);
				cairo_stroke (cr);
				cairo_pattern_destroy (pat);
			}

		}
		cairo_destroy (cr);
	}
}

}
