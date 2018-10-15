#include <cmath>
#include <lv2/lv2plug.in/ns/lv2core/lv2.h>
#include <lv2/lv2plug.in/ns/extensions/ui/ui.h>
#include "BWidgets/BWidgets.hpp"
#include "BAmp.h"
#include <iostream>
#include <cstring>

class BAmp_GUI : public BWidgets::Window
{
public:
	BAmp_GUI (PuglNativeWindow parentWindow);
	void portEvent (uint32_t port_index, uint32_t buffer_size, uint32_t format, const void* buffer);

	LV2UI_Write_Function write_function;
	LV2UI_Controller controller;
	BWidgets::Dial dial;
};

BAmp_GUI::BAmp_GUI (PuglNativeWindow parentWindow) :
		write_function (NULL), controller (NULL), BWidgets::Window (100, 100, "BAmp", parentWindow),
		dial (20, 20, 60, 60, "dial", -90.0, -90.0, 24.0, 0.0)
{
	std::cerr << "BAmp_GUI constructed.\n";
	add (dial);
	std::cerr << "Dial added.\n";
}

void BAmp_GUI::portEvent (uint32_t port_index, uint32_t buffer_size, uint32_t format, const void* buffer)
{
	if ((format == 0) && (port_index == AMP_GAIN))
	{
		float* pval = (float*)buffer;
		dial.setValue (*pval);
	}
}


LV2UI_Handle instantiate (const LV2UI_Descriptor *descriptor, const char *plugin_uri, const char *bundle_path,
						  LV2UI_Write_Function write_function, LV2UI_Controller controller, LV2UI_Widget *widget,
						  const LV2_Feature *const *features)
{
	PuglNativeWindow parentWindow = 0;
	LV2UI_Resize* resize = NULL;

	if (strcmp(plugin_uri, AMP_URI) != 0)
	{
		std::cerr << "BAmp_GUI: This GUI does not support plugin with URI " << plugin_uri << std::endl;
		return NULL;
	}

	for (int i = 0; features[i]; ++i)
	{
		if (!strcmp(features[i]->URI, LV2_UI__parent))
		{
			parentWindow = (PuglNativeWindow) features[i]->data;
		}
		else if (!strcmp(features[i]->URI, LV2_UI__resize))
		{
			resize = (LV2UI_Resize*)features[i]->data;
		}
	}
	if (parentWindow == 0) std::cerr << "BAmp_GUI: No parent window.\n";

	std::cerr << "parentXwindow: " << parentWindow << std::endl;
	BAmp_GUI* ui = new BAmp_GUI (parentWindow);

	if (ui)
	{
		ui->controller = controller;
		ui->write_function = write_function;
		if (resize)
		{
			resize->ui_resize(resize->handle, 100, 100 );
		}
		PuglNativeWindow nativeWindow = puglGetNativeWindow (ui->getPuglView ());
		*widget = (LV2UI_Widget) nativeWindow;
		std::cerr << "Widget: " << nativeWindow << std::endl;

		std::cerr << "BAmp_GUI: GUI instantiated.\n";
	}
	else std::cerr << "BAmp_GUI: Couldn't instantiate.\n";
	return (LV2UI_Handle) ui;
}

void cleanup(LV2UI_Handle ui)
{
	BAmp_GUI* pluginGui = (BAmp_GUI*) ui;
	std::cerr << "BAmp_GUI: cleanup.\n";
	delete pluginGui;
}

void portEvent(LV2UI_Handle ui, uint32_t port_index, uint32_t buffer_size, uint32_t format, const void* buffer)
{
	BAmp_GUI* pluginGui = (BAmp_GUI*) ui;
	pluginGui->portEvent(port_index, buffer_size, format, buffer);
}


const LV2UI_Descriptor guiDescriptor = {
		AMP_GUI_URI,
		instantiate,
		cleanup,
		portEvent,
		NULL	// Extension data
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

