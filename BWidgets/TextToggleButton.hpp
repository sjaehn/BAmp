#ifndef BWIDGETS_TEXTTOGGLEBUTTON_HPP_
#define BWIDGETS_TEXTTOGGLEBUTTON_HPP_

#include "ToggleButton.hpp"
#include "Label.hpp"

namespace BWidgets
{
/**
 * Class BWidgets::TextToggleButton
 *
 * Text button widget. Is is a BWidgets::ToggleButton and thus a
 * BWidgets::ValueWidget having two conditions: on (value != 0) or off
 * (value == 0)
 */
class TextToggleButton : public ToggleButton
{
public:
	TextToggleButton ();
	TextToggleButton (const double x, const double y, const double width, const double height, const std::string& name, const double defaultValue);
	~TextToggleButton ();

	/**
	 * Changes the value of the widget (0.0 == off,  0.0 != off) and relocates
	 * the embedded label widget..
	 * Emits a value changed event and (if visible) an expose event.
	 * @param val Value
	 */
	virtual void setValue (const double val) override;

	/**
	 * Gets (a pointer to) the Label for direct access.
	 * @return Pointer to the label
	 */
	Label* getLabel ();

	/**
	 * Scans theme for widget properties and applies these properties.
	 * @param theme Theme to be scanned.
	 * 				For styles used see BWidgets::Button::applyTheme and
	 * 				BWidgets::Label::applyTheme.
	 * @param name Name of the BStyles::StyleSet within the theme to be
	 * 		  	   applied.
	 */
	virtual void applyTheme (BStyles::Theme& theme);
	virtual void applyTheme (BStyles::Theme& theme, const std::string& name);

protected:
	Label buttonLabel;
};

}

#endif /* BWIDGETS_TEXTTOGGLEBUTTON_HPP_ */
