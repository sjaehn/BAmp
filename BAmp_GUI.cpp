#include <cmath>
#include <lv2/lv2plug.in/ns/lv2core/lv2.h>
#include <lv2/lv2plug.in/ns/extensions/ui/ui.h>
#include "BWidgets/BEvents/ExposeEvent.hpp"
#include "BWidgets/BWidgets/ValueDial.hpp"
#include "BAmp.h"
#include "pugl/pugl.h"
#include <iostream>
#include <cstring>

class BAmp_GUI : public BWidgets::Window
{
public:
	BAmp_GUI (PuglNativeView parentWindow);
	void portEvent (uint32_t port_index, uint32_t buffer_size, uint32_t format, const void* buffer);
	virtual void onConfigureRequest (BEvents::Event* event) override;
	static void valueChangedCallback (BEvents::Event* event);

	LV2UI_Write_Function write_function;
	LV2UI_Controller controller;
	BWidgets::ValueDial dial;
};

BAmp_GUI::BAmp_GUI (PuglNativeView parentWindow) :
	BWidgets::Window (100, 100, parentWindow, BUtilities::Urid::urid(AMP_GUI_URI), "B.Amp", true, PUGL_MODULE, 0),
	write_function (NULL), controller (NULL),
	dial (10, 10, 80, 80, 0.0, -90.0, 24.0, 0.0)
{
	dial.setClickable (false);
	add (&dial);
	dial.setCallbackFunction (BEvents::Event::EventType::valueChangedEvent, BAmp_GUI::valueChangedCallback);
}

void BAmp_GUI::portEvent (uint32_t port_index, uint32_t buffer_size, uint32_t format, const void* buffer)
{
	if ((format == 0) && (port_index == AMP_GAIN))
	{
		float* pval = (float*)buffer;
		dial.setValue (*pval);
	}
}

void BAmp_GUI::onConfigureRequest (BEvents::Event* event)
{
	Window::onConfigureRequest (event);

	double sz = (getWidth() > getHeight() ? getHeight() : getWidth()) / 100;
	dial.label.setFont(BStyles::Font("Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL, 12.0 * sz));
	dial.moveTo (10 * sz, 10 * sz);
	dial.resize (80 * sz, 80 * sz);
}

void BAmp_GUI::valueChangedCallback (BEvents::Event* event)
{
	if ((event) && (event->getWidget ()))
	{
		BWidgets::Widget* widget = event->getWidget ();
		if (!widget) return;
		BWidgets::ValueableTyped<double>* vd = dynamic_cast<BWidgets::ValueableTyped<double>*>(widget);
		if (!vd) return;
		float value = vd->getValue ();

		if (widget->getMainWindow ())
		{
			BAmp_GUI* ui = (BAmp_GUI*) widget->getMainWindow ();

			if (widget == (BWidgets::Widget*) &ui->dial)
			{
				ui->write_function(ui->controller, AMP_GAIN, sizeof(float), 0, &value);
			}
		}
	}
}

static LV2UI_Handle instantiate (const LV2UI_Descriptor *descriptor, const char *plugin_uri, const char *bundle_path,
						  LV2UI_Write_Function write_function, LV2UI_Controller controller, LV2UI_Widget *widget,
						  const LV2_Feature *const *features)
{
	PuglNativeView parentWindow = 0;

	if (strcmp(plugin_uri, AMP_URI) != 0)
	{
		std::cerr << "BAmp_GUI: This GUI does not support plugin with URI " << plugin_uri << std::endl;
		return NULL;
	}

	for (int i = 0; features[i]; ++i)
	{
		if (!strcmp(features[i]->URI, LV2_UI__parent)) parentWindow = (PuglNativeView) features[i]->data;
	}
	if (parentWindow == 0) std::cerr << "BAmp_GUI: No parent window.\n";

	BAmp_GUI* ui = new BAmp_GUI (parentWindow);

	if (ui)
	{
		ui->controller = controller;
		ui->write_function = write_function;
		*widget = (LV2UI_Widget) ui->getNativeView ();
	}
	else std::cerr << "BAmp_GUI: Couldn't instantiate.\n";
	return (LV2UI_Handle) ui;
}

static void cleanup(LV2UI_Handle ui)
{
	BAmp_GUI* pluginGui = (BAmp_GUI*) ui;
	delete pluginGui;
}

static void portEvent(LV2UI_Handle ui, uint32_t port_index, uint32_t buffer_size, uint32_t format, const void* buffer)
{
	BAmp_GUI* pluginGui = (BAmp_GUI*) ui;
	pluginGui->portEvent(port_index, buffer_size, format, buffer);
}

static int callIdle(LV2UI_Handle ui)
{
	BAmp_GUI* pluginGui = (BAmp_GUI*) ui;
	pluginGui->handleEvents ();
	return 0;
}

static const LV2UI_Idle_Interface idle = {callIdle};

static const void* extensionData(const char* uri)
{
	if (!strcmp(uri, LV2_UI__idleInterface)) return &idle;
	else return NULL;
}


static const LV2UI_Descriptor guiDescriptor = {
		AMP_GUI_URI,
		instantiate,
		cleanup,
		portEvent,
		extensionData
};

// LV2 Symbol Export
LV2_SYMBOL_EXPORT const LV2UI_Descriptor *lv2ui_descriptor(uint32_t index)
{
	switch (index)
	{
	case 0: return &guiDescriptor;
	default:return NULL;
    }
}
