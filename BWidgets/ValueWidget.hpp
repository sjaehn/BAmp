#ifndef BWIDGETS_VALUEWIDGET_HPP_
#define BWIDGETS_VALUEWIDGET_HPP_

#include "Widget.hpp"

namespace BWidgets
{

/**
 * Class BWidgets::ValueWidget
 *
 * Root class for all widgets that deal with values (dials, sliders, ...).
 */
class ValueWidget : public Widget
{
public:
	ValueWidget ();
	ValueWidget (const double x, const double y, const double width, const double height, const std::string& name, const double value);
	~ValueWidget ();

	/**
	 * Changes the value of the widget. Emits a value changed event and (if
	 * visible) an expose event.
	 * @param val Value
	 */
	virtual void setValue (const double val);

	/**
	 * Gets the value of the widget.
	 * @return Value
	 */
	double getValue () const;

protected:
	void postValueChanged ();
	double value;
};

}

#endif /* BWIDGETS_VALUEWIDGET_HPP_ */
