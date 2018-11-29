/* Widget.hpp
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

#ifndef BWIDGETS_WIDGET_HPP_
#define BWIDGETS_WIDGET_HPP_

// Default basic widget settings
#define BWIDGETS_DEFAULT_BORDER BStyles::noBorder
#define BWIDGETS_DEFAULT_BACKGROUND BStyles::noFill
#define BWIDGETS_DEFAULT_WIDTH 200
#define BWIDGETS_DEFAULT_HEIGHT 200
#define BWIDGETS_DEFAULT_STATE BColors::NORMAL
#define BWIDGETS_DEFAULT_FGCOLORS BColors::greens
#define BWIDGETS_DEFAULT_BGCOLORS BColors::darks
#define BWIDGETS_DEFAULT_FONT BStyles::sans12pt
#define BWIDGETS_DEFAULT_ILLUMINATED 0.333
#define BWIDGETS_DEFAULT_NORMALLIGHTED 0.0
#define BWIDGETS_DEFAULT_SHADOWED -0.333
#define BWIDGETS_DEFAULT_DARKENED -0.5

// Default BWidgets::Window settings (Note: use non-transparent backgrounds only)
#define BWIDGETS_DEFAULT_WINDOW_BACKGROUND BStyles::blackFill

// Default settings for all text containing widgets
#define BWIDGETS_DEFAULT_TEXT_COLORS BColors::lights
#define BWIDGETS_DEFAULT_TEXT_ALIGN BStyles::TEXT_ALIGN_LEFT
#define BWIDGETS_DEFAULT_TEXT_VALIGN BStyles::TEXT_VALIGN_TOP

// Default settings for all menu widgets
#define BWIDGETS_DEFAULT_MENU_PADDING 10
#define BWIDGETS_DEFAULT_MENU_BORDER BStyles::greyBorder1pt
#define BWIDGETS_DEFAULT_MENU_BACKGROUND BStyles::grey20Fill

// BWidgets theme keywords
#define BWIDGETS_KEYWORD_BORDER "border"
#define BWIDGETS_KEYWORD_BACKGROUND "background"
#define BWIDGETS_KEYWORD_FONT "font"
#define BWIDGETS_KEYWORD_FGCOLORS "fgcolors"
#define BWIDGETS_KEYWORD_BGCOLORS "bgcolors"
#define BWIDGETS_KEYWORD_TEXTCOLORS "textcolors"


#include <cairo/cairo.h>
#include "cairoplus.h"
#include "pugl/pugl.h"
#include <stdint.h>
#include <vector>
#include <string>
#include <iostream>
#include <functional>

#include "BColors.hpp"
#include "BStyles.hpp"
#include "BEvents.hpp"
#include "BValues.hpp"

namespace BWidgets
{
/**
 * Class BWidgets::Widget
 *
 * Root widget class of BWidgets. All other widget (including BWidgets::Window)
 * are derived from this class.
 * A BWidgets::Widget (and all derived widgets) are drawable and may have a
 * border and a background. A BWidgets::Widget (and all derived widgets) may
 * also be containers for other widgets (= have children).
 */
class Window; // Forward declaration

class Widget
{
public:
	Widget ();
	Widget (const double x, const double y, const double width, const double height);
	Widget (const double x, const double y, const double width, const double height, const std::string& name);

	/**
	 * Creates a new (orphan) widget and copies the widget properties from a
	 * source widget. This method doesn't copy any parent or child widgets.
	 * @param that Source widget
	 */
	Widget (const Widget& that);

	virtual ~Widget ();

	/**
	 * Assignment. Copies the widget properties from a source widget and keeps
	 * its name and its position within the widget tree. Emits a
	 * BEvents::ExposeEvent if the widget is visible.
	 * @param that Source widget
	 */
	Widget& operator= (const Widget& that);

	/**
	 * Makes the widget visible (if its parents are visible too) and emits an
	 * BEvents::ExposeEvent to the main window.
	 */
	void show ();

	/**
	 * Makes the widget invisible and emits an
	 * BEvents::ExposeEvent to the main window.
	 */
	void hide ();

	/**
	 * Adds a child widget to the widget (container function). A
	 * BEvents::ExposeEvent will be emitted if the child widget will be
	 * unveiled due to the addition.
	 * @param child Child widget
	 */
	void add (Widget& child);

	/**
	 * Releases (but not deletes) a child widget from the container of this
	 * widget. A BEvents::ExposeEvent will be emitted if a visible child is
	 * released.
	 * @param child Pointer to the child widget
	 */
	void release (Widget* child);

	/**
	 * Moves the widget and emits a BEvents::ExposeEvent if the widget is
	 * visible.
	 * @param x New x coordinate
	 * @param y New y coordinate
	 */
	void moveTo (const double x, const double y);

	/**
	 * Gets the widgets x position
	 * @return X position
	 */
	double getX () const;

	/**
	 * Gets the widgets y position
	 * @return Y position
	 */
	double getY () const;

	/**
	 * Gets the widgets x position relative to the position of its main window.
	 * @return X position
	 */
	double getOriginX ();

	/**
	 * Gets the widgets y position relative to the position of its main window.
	 * @return Y position
	 */
	double getOriginY ();

	/**
	 * Pushes this widget backwards (to the background) if it is linked to a
	 * parent widget. Emits a BEvents::ExposeEvent if the widget is visible.
	 */
	void moveBackwards ();

	/**
	 * Pulls this widget frontwards (to the front) if it is linked to a
	 * parent widget. Emits a BEvents::ExposeEvent if the widget is visible.
	 */
	void moveFrontwards ();

	/**
	 * Resizes the widget, redraw and emits a BEvents::ExposeEvent if the
	 * widget is visible.
	 * @param width New widgets width
	 */
	virtual void setWidth (const double width);

	/**
	 * Gets the width of the widget
	 * @return Width
	 */
	double getWidth () const;

	/**
	 * Resizes the widget, redraw and emits a BEvents::ExposeEvent if the
	 * widget is visible.
	 * @param height New widgets height
	 */
	virtual void setHeight (const double height);

	/**
	 * Gets the height of the widget
	 * @return Height
	 */
	double getHeight () const;

	/**
	 * Gets the x offset of the widget content. This is distance between the
	 * outer border and the widget content. It is also the sum of margin,
	 * border, and padding.
	 * @return X offset of the widget
	 */
	double getXOffset ();

	/**
	 * Gets the y offset of the widget content. This is distance between the
	 * outer border and the widget content. It is also the sum of margin,
	 * border, and padding.
	 * @return Y offset of the widget
	 */
	double getYOffset ();

	/**
	 * Gets the effective width of the widget content without its borders.
	 * @return Effective width of the widget
	 */
	double getEffectiveWidth ();

	/**
	 * Gets the effective height of the widget content without its borders.
	 * @return Effective height of the widget
	 */
	double getEffectiveHeight ();


	/**
	 * Sets the widgets state
	 * @param state Widget state
	 */
	void setState (const BColors::State state);

	/**
	 * Gets the widgets state
	 * @return Widget state
	 */
	BColors::State getState () const;

	/**
	 * (Re-)Defines the border of the widget. Redraws widget and emits a
	 * BEvents::ExposeEvent if the widget is visible.
	 * @param border New widgets border
	 */
	virtual void setBorder (const BStyles::Border& border);

	/**
	 * Gets (a pointer to) the border of the widget.
	 * @return Pointer to BStyles::Border
	 */
	BStyles::Border* getBorder ();

	/**
	 * (Re-)Defines the background of the widget. Redraws widget and emits a
	 * BEvents::ExposeEvent if the widget is visible.
	 * @param background New widgets background
	 */
	void setBackground (const BStyles::Fill& background);

	/**
	 * Gets (a pointer to) the background of the widget.
	 * @return Pointer to BStyles::Fill
	 */
	BStyles::Fill* getBackground ();

	/**
	 * Gets a pointer to the widgets main window.
	 * @return Pointer to the main window. Returns nullptr if the widget
	 * isn't connected to a main window.
	 */
	Window* getMainWindow () const;

	/**
	 * Gets a pointer to the widgets parent widget.
	 * @return Pointer to the parent widget. Returns nullptr if the widget
	 * doesn't have any parents.
	 */
	Widget* getParent () const;

	/**
	 * Tests whether the widget has children or not.
	 * @return TRUE if the widget has children, otherwise FALSE
	 */
	bool hasChildren () const;

	/**
	 * Gets the widgets children vector. The vector contains all children of
	 * the widgets from background to foreground.
	 * @return Children vector.
	 */
	std::vector<Widget*> getChildren () const;

	/**
	 * Renames the widget.
	 * @param name New name
	 */
	void rename (const std::string& name);

	/**
	 * Gets the name of the widget
	 * @return Name of the widget
	 */
	std::string getName () const;

	/**
	 * Gets the visibility of the widget. Therefore, all its parents will be
	 * checked for visibility too.
	 * @return TRUE if the widget (and all its parents) are visible, otherwise
	 * 		   FALSE.
	 */
	bool isVisible ();

	/**
	 * Defines whether the widget may emit BEvents::BUTTON_PRESS_EVENT's
	 * following a host button event.
	 * @param status TRUE if widget is clickable, otherwise false
	 */
	void setClickable (const bool status);

	/**
	 * Gets whether the widget may emit BEvents::BUTTON_PRESS_EVENT's or
	 * BEvents::BUTTON_RELEASE_Event's following a host button event.
	 * @return TRUE if widget is clickable, otherwise false
	 */
	bool isClickable () const;

	/**
	 * Defines whether the widget may emit
	 * BEvents::POINER_MOTION_WHILE_BUTTON_PRESSED_EVENT's following a host
	 * button event.
	 * @param status TRUE if widget is clickable, otherwise false
	 */
	void setDragable (const bool status);

	/**
	 * Gets whether the widget may emit BEvents::BUTTON_PRESS_EVENT's or
	 * BEvents::BUTTON_RELEASE_Event's following a host button event.
	 * @return TRUE if widget is clickable, otherwise false
	 */
	bool isDragable () const;

	/**
	 * Calls a redraw of the widget and calls postRedisplay () if the the
	 * Widget is visible.
	 * This method should be called if the widgets properties are indirectly
	 * changed.
	 */
	virtual void update ();

	/**
	 * Requests a redisplay of the widgets area (and all underlying widget
	 * areas) by emitting a BEvents::ExposeEvent.
	 */
	void postRedisplay ();

	/**
	 * (Re-)Defines the callback function for an event. It is on the onXXX
	 * methods whether a callback function will be called or not. By default,
	 * the callback is set to defaultCallback.
	 * @param eventType Enumeration of the event type.
	 * @param callbackFunction Function that should be called if an onXXX
	 * 						   method is called as response of an event and
	 * 						   onXXX allows callbacks.
	 */
	void setCallbackFunction (const BEvents::EventType eventType, const std::function<void (BEvents::Event*)>& callbackFunction);

	/**
	 * Predefined empty callback function
	 * @param event Placeholder, will not be interpreted by this method.
	 */
	static void defaultCallback (BEvents::Event* event);

	/**
	 * Predefined empty method to handle a BEvents::EventType::CONFIGURE_EVENT.
	 * BEvents::EventType::CONFIGURE_EVENTs will only be handled by
	 * BWidget::Window.
	 */
	virtual void onConfigure (BEvents::ExposeEvent* event);

	/**
	 * Predefined empty method to handle a BEvents::EventType::EXPOSE_EVENT.
	 * BEvents::EventType::EXPOSE_EVENTs will only be handled by
	 * BWidget::Window.
	 * @param event Expose event
	 */
	virtual void onExpose (BEvents::ExposeEvent* event);

	/**
	 * Predefined empty method to handle a BEvents::EventType::CLOSE_EVENT.
	 * BEvents::EventType::CLOSE_EVENTs will only be handled by
	 * BWidget::Window.
	 */
	virtual void onClose ();

	/**
	 * Predefined empty method to handle a
	 * BEvents::EventType::BUTTON_PRESS_EVENT.
	 */
	virtual void onButtonPressed (BEvents::PointerEvent* event);

	/**
	 * Predefined empty method to handle a
	 * BEvents::EventType::BUTTON_RELEASE_EVENT.
	 * @param event Pointer event
	 */
	virtual void onButtonReleased (BEvents::PointerEvent* event);

	/**
	 * Predefined empty method to handle a
	 * BEvents::EventType::POINTER_MOTION_EVENT.
	 * @param event Pointer event
	 */
	virtual void onPointerMotion (BEvents::PointerEvent* event);

	/**
	 * Predefined empty method to handle a
	 * BEvents::EventType::POINTER_MOTION_WHILE_BUTTON_PRESSED_EVENT.
	 * @param event Pointer event
	 */
	virtual void onPointerMotionWhileButtonPressed (BEvents::PointerEvent* event);

	/**
	 * Predefined empty method to handle a
	 * BEvents::EventType::VALUE_CHANGED_EVENT.
	 * @param event Value changed event
	 */
	virtual void onValueChanged (BEvents::ValueChangedEvent* event);

	/**
	 * Scans theme for widget properties and applies these properties.
	 * @param theme Theme to be scanned
	 */
	virtual void applyTheme (BStyles::Theme& theme);

	/**
	 * Scans theme for widget properties and applies these properties.
	 * @param theme Theme to be scanned
	 * @param name Name of the BStyles::StyleSet within the theme to be
	 * 		  	   applied.
	 */
	virtual void applyTheme (BStyles::Theme& theme, const std::string& name);


protected:

	/**
	 * Linearizes the whole children tree.
	 * @param queue Vector to which all (pointers to) children shall be added.
	 * 				Default = empty.
	 * @return Vector of pointers to all children.
	 */
	std::vector <Widget*> getChildrenAsQueue (std::vector <Widget*> queue = {}) const;

	bool isPointInWidget (const double x, const double y) const;
	Widget* getWidgetAt (const double x, const double y, const bool checkVisibility, const bool checkClickability, const bool checkDragability);

	void postRedisplay (const double x, const double y, const double width, const double height);
	void redisplay (cairo_surface_t* surface, double x, double y, double width, double height);

	virtual void draw (const double x, const double y, const double width, const double height);

	bool fitToArea (double& x, double& y, double& width, double& height);

	long id;
	double x_, y_, width_, height_;
	bool visible;
	bool clickable;
	bool dragable;
	Window* main_;
	Widget* parent_;
	std::vector <Widget*> children_;
	BStyles::Border border_;
	BStyles::Fill background_;
	std::string name_;
	std::array<std::function<void (BEvents::Event*)>, BEvents::EventType::NO_EVENT> cbfunction;
	cairo_surface_t* widgetSurface;
	BColors::State widgetState;
};

/**
 * Class BWidgets::Window
 *
 * Main window class of BWidgets. Add all other widgets (directly or
 * indirectly) to this window.
 * A BWidgets::Window is the BWidgets::Widget that is controlled by the host
 * via Pugl, receives host events via Pugl and coordinates handling of all
 * events. Configure, expose, and close events will be handled directly and
 * exclusively by this widget.
 */
class Window : public Widget
{
public:
	Window ();
	Window (const double width, const double height, const std::string& title, PuglNativeWindow nativeWindow, bool resizable = false);

	Window (const Window& that) = delete;	// Only one window in this version

	~Window ();

	Window& operator= (const Window& that) = delete;	// Only one Window in this version

	/**
	 * Gets in contact to the host system via Pugl
	 * @return Pointer to the PuglView
	 */
	PuglView* getPuglView ();

	/**
	 * Gets the Cairo context provided by the host system via Pugl
	 * @return Pointer to the Cairo context
	 */
	cairo_t* getPuglContext ();

	/**
	 * Runs the window until the close flag is set and thus it will be closed.
	 * For stand-alone applications.
	 */
	void run ();

	/**
	 * Queues an event until the next call of the handleEvents method.
	 * @param event Event
	 */
	void addEventToQueue (BEvents::Event* event);

	/**
	 * Main Event handler. Walks through the event queue and sorts the events
	 * to their respective onXXX handling methods
	 */
	void handleEvents ();

	/**
	 * Executes an reexposure of the area given by the expose event.
	 * @param event Expose event containing the widget that emitted the event
	 * 				and the area that should be reexposed.
	 */
	virtual void onExpose (BEvents::ExposeEvent* event) override;

	/**
	 * Predefined empty method to handle a BEvents::EventType::CONFIGURE_EVENT.
	 * BEvents::EventType::CONFIGURE_EVENTs will only be handled by
	 * BWidget::Window.
	 */
	virtual void onConfigure (BEvents::ExposeEvent* event) override;

	/**
	 * Sets the close flag and thus ends the run method.
	 */
	virtual void onClose () override;

	/*
	 * Links or unlinks a mouse button to a widget.
	 * @param device Button
	 * @param widget Pointer to the widget to be linked or nullptr to unlink
	 */
	void setInput (const BEvents::InputDevice device, Widget* widget);

	/*
	 * Gets the links from mouse button to a widget.
	 * @param device Button
	 * @return Pointer to the linked widget or nullptr
	 */
	Widget* getInput (BEvents::InputDevice device) const;

protected:

	/**
	 * Communication interface to the host via Pugl. Translates PuglEvents to
	 * BEvents::Event derived objects.
	 */
	static void translatePuglEvent (PuglView* view, const PuglEvent* event);

	void purgeEventQueue ();

	std::string title_;
	PuglView* view_;
	PuglNativeWindow nativeWindow_;
	bool quit_;

	double pointerX;
	double pointerY;

	/**
	 * Stores either nullptr or (a pointer to) the widget that emitted the
	 * BEvents::BUTTON_PRESS_EVENT until a BEvents::BUTTON_RELEASE_EVENT or
	 * the linked widget is released or destroyed.
	 */
	std::array<Widget*, BEvents::InputDevice::NR_OF_BUTTONS> input;
	std::vector<BEvents::Event*> eventQueue;
};

}



#endif /* BWIDGETS_WIDGET_HPP_ */
