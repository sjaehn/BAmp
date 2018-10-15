#include "DialWithValueDisplay.hpp"

namespace BWidgets
{

DialWithValueDisplay::DialWithValueDisplay () :
		DialWithValueDisplay (0.0, 0.0, 50.0, 60.0, "valuedial", 0.0, 0.0, 100.0, 1.0, "%3.2f") {}

DialWithValueDisplay::DialWithValueDisplay (const double x, const double y, const double width, const double height, const std::string& name,
											const double value, const double min, const double max, const double step,
											const std::string& valueFormat) :
	RangeWidget (x, y, width, height, name, value, min, max, step),
	dial (0, 0, width, width, name, value, min, max, step),
	valueDisplay(0, 0.75 * height, width, 0.25 * height, name),
	valFormat (valueFormat)
{
	dial.setCallbackFunction (BEvents::EventType::VALUE_CHANGED_EVENT, DialWithValueDisplay::redirectPostValueChanged);
	update ();
	add (dial);
	add (valueDisplay);
}

DialWithValueDisplay::~DialWithValueDisplay () {}

void DialWithValueDisplay::setValueFormat (const std::string& valueFormat)
{
	valFormat = valueFormat;
	update ();
}

std::string DialWithValueDisplay::getValueFormat () const {return valFormat;}

Dial* DialWithValueDisplay::getDial () {return &dial;}

Label* DialWithValueDisplay::getValueDisplay () {return &valueDisplay;}

void DialWithValueDisplay::update ()
{
	updateChildCoords ();
	draw (0, 0, width_, height_);
	if (isVisible ()) postRedisplay ();
}

void DialWithValueDisplay::applyTheme (BStyles::Theme& theme) {applyTheme (theme, name_);}
void DialWithValueDisplay::applyTheme (BStyles::Theme& theme, const std::string& name)
{
	Widget::applyTheme (theme, name);
	dial.applyTheme (theme, name);
	valueDisplay.applyTheme (theme, name);
	update ();
}

void DialWithValueDisplay::updateChildCoords ()
{
	double w = (height_ > width_ ? width_ : height_);
	double h = (height_ > width_ ? width_ : height_);
	dial.moveTo (0.5 * width_ - 0.5 * w, 0.5 * height_ - 0.5 * w);
	dial.setWidth (w);
	dial.setHeight (w);

	valueDisplay.moveTo (0.5 * width_ - 0.5 * w, 0.5 * height_ + 0.3 * h);
	valueDisplay.setWidth (w);
	valueDisplay.setHeight (0.25 * h);
	valueDisplay.getFont ()->setFontSize (0.2 * w);
	valueDisplay.setText (BValues::toBString (valFormat, value));
	valueDisplay.update ();
}

void DialWithValueDisplay::redirectPostValueChanged (BEvents::Event* event)
{
	if (event && (event->getEventType () == BEvents::EventType::VALUE_CHANGED_EVENT) && event->getWidget ())
	{
		BEvents::ValueChangedEvent* ev = (BEvents::ValueChangedEvent*) event;
		Widget* w = (Widget*) ev->getWidget ();
		if (w->getParent ())
		{
			DialWithValueDisplay* p = (DialWithValueDisplay*) w->getParent ();
			p->setValue (ev->getValue ());
			p->getValueDisplay ()->setText(BValues::toBString (p->getValueFormat (), p->getValue ()));
		}
	}
}

void DialWithValueDisplay::draw (const double x, const double y, const double width, const double height)
{
	// Draw super class widget elements only
	Widget::draw (x, y, width, height);
}

}
