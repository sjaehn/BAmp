#ifndef BWIDGETS_DIALWITHVALUEDISPLAY_HPP_
#define BWIDGETS_DIALWITHVALUEDISPLAY_HPP_

#include "BWidgets.hpp"

namespace BWidgets
{
/**
 * Class BWidgets::DialWithValueDisplay
 *
 * Composite RangeWidget consisting of a dial and a label widget.
 */
class DialWithValueDisplay : public RangeWidget
{
public:
	DialWithValueDisplay ();
	DialWithValueDisplay (const double x, const double y, const double width, const double height, const std::string& name,
						  const double value, const double min, const double max, const double step, const std::string& valueFormat);
	~DialWithValueDisplay ();

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
	 * Gets (a pointer to) the dial for direct access.
	 * @return Pointer to the dial
	 */
	Dial* getDial ();

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
	void updateChildCoords ();
	static void redirectPostValueChanged (BEvents::Event* event);
	virtual void draw (const double x, const double y, const double width, const double height) override;

	Dial dial;
	Label valueDisplay;

	std::string valFormat;
};

/*****************************************************************************/

}



#endif /* BWIDGETS_DIALWITHVALUEDISPLAY_HPP_ */
