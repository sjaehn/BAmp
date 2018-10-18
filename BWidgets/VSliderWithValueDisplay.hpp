#ifndef BWIDGETS_VSLIDERWITHVALUEDISPLAY_HPP_
#define BWIDGETS_VSLIDERWITHVALUEDISPLAY_HPP_

#include "RangeWidget.hpp"
#include "Label.hpp"
#include "VSlider.hpp"

namespace BWidgets
{

#ifndef BWIDGETS_VALUEDISPLAYPOSITION
#define BWIDGETS_VALUEDISPLAYPOSITION
/**
 * Enumeration of positions where the value should appear
 */
typedef enum {
	ON_TOP,
	ON_BOTTOM,
	ON_LEFT,
	ON_RIGHT
} ValueDisplayPosition;
#endif


/**
 * Class BWidgets::VSliderWithValueDisplay
 *
 * Composite RangeWidget consisting of a vertical slider and a label widget.
 */
class VSliderWithValueDisplay : public RangeWidget
{
public:
	VSliderWithValueDisplay ();
	VSliderWithValueDisplay (const double x, const double y, const double width, const double height, const std::string& name,
			  	  	  	  	 const double value, const double min, const double max, const double step,
							 const std::string& valueFormat, const ValueDisplayPosition position);
	~VSliderWithValueDisplay ();

	/**
	 * Changes the value of the widget and keeps it within the defined range.
	 * Passes the value to its predefined child widgets.
	 * Emits a value changed event and (if visible) an expose event.
	 * @param val Value
	 */
	virtual void setValue (const double val) override;

	/**
	 * Sets the lower limit. Forces the value into the new range. Passes the
	 * min to its predefined child widgets. Emits a value changed event (if
	 * value changed) and (if visible) an expose event.
	 * @param min Lower limit
	 */
	virtual void setMin (const double min) override;

	/**
	 * Sets the upper limit. Forces the value into the new range. Passes the
	 * max to its predefined child widgets. Emits a value changed event (if
	 * value changed) and (if visible) an expose event.
	 * @param max Upper limit
	 */
	virtual void setMax (const double min) override;

	/**
	 * Sets the increment steps for the value. Passes the
	 * increment to its predefined child widgets.
	 * @param step Increment steps.
	 */
	virtual void setStep (const double step);

	/**
	 * Sets the value output format.
	 * @valueFormat Format of the output in printf standard for type double.
	 */
	void setValueFormat (const std::string& valueFormat);

	/**
	 * Gets the value output format.
	 * @return Format of the output in printf standard for type double.
	 */
	std::string getValueFormat () const;

	/**
	 * Sets the position where the value should appear
	 * @param position Enumeration of position
	 */
	void setValuePosition (const ValueDisplayPosition position);

	/**
	 * Gets the position where the value should appear
	 * @return Enumeration of position
	 */
	ValueDisplayPosition getValuePosition () const;

	/**
	 * Gets (a pointer to) the slider widget for direct access
	 * @return Pointer to BWidgets::VSlider
	 */
	VSlider* getSlider ();

	/**
	 * Gets (a pointer to) the Label for direct access.
	 * @return Pointer to the label
	 */
	Label* getValueDisplay ();

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
	 * 				For styles used see BWidgets::Dial::applyTheme and
	 * 				BWidgets::Label::applyTheme.
	 * @param name Name of the BStyles::StyleSet within the theme to be
	 * 		  	   applied.
	 */
	virtual void applyTheme (BStyles::Theme& theme);
	virtual void applyTheme (BStyles::Theme& theme, const std::string& name);

protected:
	static void redirectPostValueChanged (BEvents::Event* event);
	void updateChildCoords ();
	virtual void draw (const double x, const double y, const double width, const double height) override;

	VSlider slider;
	Label valueDisplay;

	std::string valFormat;
	ValueDisplayPosition valPosition;
};

}

#endif /* BWIDGETS_VSLIDERWITHVALUEDISPLAY_HPP_ */
