BUNDLE = BAmp.lv2
PREFIX ?= /usr/local
LV2DIR ?= $(PREFIX)/lib/lv2
CXX ?= g++
DSPFLAGS = -shared -fPIC -DPIC `pkg-config --cflags --libs lv2`
GUIFLAGS = -shared -DPUGL_HAVE_CAIRO -fPIC -DPIC `pkg-config --cflags --libs lv2 x11 cairo`
TK = BWidgets/DrawingSurface.cpp BWidgets/DialValue.cpp BWidgets/Dial.cpp BWidgets/Knob.cpp BWidgets/RangeWidget.cpp BWidgets/ValueWidget.cpp BWidgets/Label.cpp BWidgets/FocusWidget.cpp BWidgets/Window.cpp BWidgets/Widget.cpp BWidgets/BStyles.cpp BWidgets/BColors.cpp BWidgets/BEvents.cpp BWidgets/BValues.cpp BWidgets/cairoplus.c BWidgets/pugl/pugl_x11_cairo.c BWidgets/pugl/pugl_x11.c

$(BUNDLE): clean BAmp.so BAmp_GUI.so
	cp manifest.ttl BAmp.ttl $(BUNDLE)

all: $(BUNDLE)

BAmp.so: ./BAmp.cpp
	mkdir -p $(BUNDLE)
	$(CXX) $< -o $(BUNDLE)/$@ $(CXXFLAGS) $(LDFLAGS) $(DSPFLAGS)

BAmp_GUI.so: ./BAmp_GUI.cpp
	mkdir -p $(BUNDLE)
	$(CXX) $< $(TK) -o $(BUNDLE)/$@ $(CXXFLAGS) $(LDFLAGS) $(GUIFLAGS)

install:
	mkdir -p $(DESTDIR)$(LV2DIR)
	rm -rf $(DESTDIR)$(LV2DIR)/$(BUNDLE)
	cp -R $(BUNDLE) $(DESTDIR)$(LV2DIR)

.PHONY: all

clean:
	rm -rf $(BUNDLE)
