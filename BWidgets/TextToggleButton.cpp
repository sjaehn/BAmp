#include "TextToggleButton.hpp"

namespace BWidgets
{
TextToggleButton::TextToggleButton () : TextToggleButton (0.0, 0.0, 50.0, 50.0, "texttogglebutton", 0.0) {}

TextToggleButton::TextToggleButton (const double x, const double y, const double width, const double height, const std::string& name, const double defaultValue) :
		ToggleButton (x, y, width, height, name, defaultValue), buttonLabel (0, 0, width - 2, height - 2, name)
{
	buttonLabel.setClickable  (false);
	setValue (defaultValue);
	add (buttonLabel);
}

TextToggleButton:: ~TextToggleButton () {}

void TextToggleButton::setValue (const double val)
{
	if (val) buttonLabel.moveTo (2, 2);
	else buttonLabel.moveTo (0, 0);
	ToggleButton::setValue (val);
}

Label* TextToggleButton::getLabel () {return &buttonLabel;}

void TextToggleButton::applyTheme (BStyles::Theme& theme) {applyTheme (theme, name_);}
void TextToggleButton::applyTheme (BStyles::Theme& theme, const std::string& name)
{
	ToggleButton::applyTheme (theme, name);
	buttonLabel.applyTheme (theme, name);
	update ();
}

}
