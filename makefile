BUNDLE = BAmp.lv2
INSTALL_DIR = /usr/lib/lv2
CXX = g++
TK = BWidgets/DrawingSurface.cpp BWidgets/DisplayDial.cpp BWidgets/Dial.cpp BWidgets/Knob.cpp BWidgets/RangeWidget.cpp BWidgets/ValueWidget.cpp BWidgets/Label.cpp BWidgets/FocusWidget.cpp BWidgets/Window.cpp BWidgets/Widget.cpp BWidgets/BStyles.cpp BWidgets/BColors.cpp BWidgets/BEvents.cpp BWidgets/BValues.cpp BWidgets/cairoplus.c BWidgets/pugl/pugl_x11_cairo.c BWidgets/pugl/pugl_x11.c

$(BUNDLE): clean BAmp.so BAmp_GUI.so
	cp manifest.ttl BAmp.ttl $(BUNDLE)

all: $(BUNDLE)

BAmp.so: ./BAmp.cpp
	mkdir -p $(BUNDLE)
	$(CXX) $< -o $(BUNDLE)/$@ -shared -fPIC -DPIC `pkg-config --cflags --libs lv2`

BAmp_GUI.so: ./BAmp_GUI.cpp
	mkdir -p $(BUNDLE)
	$(CXX) $< $(TK) -o $(BUNDLE)/$@ -shared -DPUGL_HAVE_CAIRO -fPIC -DPIC `pkg-config --cflags --libs lv2 x11 cairo`

install:
	mkdir -p $(INSTALL_DIR)
	rm -rf $(INSTALL_DIR)/$(BUNDLE)
	cp -R $(BUNDLE) $(INSTALL_DIR)

.PHONY: all

clean:
	rm -rf $(BUNDLE)
