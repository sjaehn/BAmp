#ifndef BWIDGETS_TOGGLEBUTTON_HPP_
#define BWIDGETS_TOGGLEBUTTON_HPP_

#include "Button.hpp"

namespace BWidgets
{
/**
 * Class BWidgets::ToggleButton
 *
 * Basic toggle button widget. Is is a BWidgets::Button and thus a
 * BWidgets::ValueWidget having two conditions: on (value != 0) or off
 * (value == 0)
 */
class ToggleButton : public Button
{
public:
	ToggleButton ();
	ToggleButton (const double x, const double y, const double width, const double height, const std::string& name, const double defaultValue);
	~ToggleButton ();

	/**
	 * Handles the BEvents::BUTTON_PRESS_EVENT.
	 * @param event Pointer to a pointer event emitted by the same widget.
	 */
	virtual void onButtonPressed (BEvents::PointerEvent* event) override;

	/**
	 * Overrides the BEvents::BUTTON_RELEASED_EVENT handled by
	 * BWidgets::Button.
	 * @param event Pointer to a pointer event emitted by the same widget.
	 */
	virtual void onButtonReleased (BEvents::PointerEvent* event) override;
};

}




#endif /* BWIDGETS_TOGGLEBUTTON_HPP_ */
