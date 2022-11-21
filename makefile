BUNDLE = BAmp.lv2
PREFIX ?= /usr/local
LV2DIR ?= $(PREFIX)/lib/lv2

CC ?= gcc
CXX ?= g++

CPPFLAGS += -DPIC
CFLAGS += -std=c99 -fvisibility=hidden -fPIC
CXXFLAGS += -std=c++11 -fvisibility=hidden -fPIC
LDFLAGS += -shared
PUGLFLAGS += -DPUGL_HAVE_CAIRO

DSPFLAGS = `pkg-config --cflags --libs lv2`
GUICFLAGS = `pkg-config --cflags lv2 x11 cairo`
GUILFLAGS = `pkg-config --libs lv2 x11 cairo`

TKCXX =	BWidgets/BWidgets/Window.cpp \
	BWidgets/BWidgets/Widget.cpp \
	BWidgets/BWidgets/Supports/Closeable.cpp \
	BWidgets/BWidgets/Supports/Messagable.cpp \
	BWidgets/BUtilities/Urid.cpp

TKC = BWidgets/BWidgets/pugl/implementation.c \
	BWidgets/BWidgets/pugl/x11_stub.c \
	BWidgets/BWidgets/pugl/x11_cairo.c \
	BWidgets/BWidgets/pugl/x11.c \
	BWidgets/BUtilities/cairoplus.c

$(BUNDLE): clean BAmp.so BAmp_GUI.so
	cp manifest.ttl BAmp.ttl $(BUNDLE)

all: $(BUNDLE)

BAmp.so: ./BAmp.cpp
	mkdir -p $(BUNDLE)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) $(DSPFLAGS) $< -o $(BUNDLE)/$@

BAmp_GUI.so: ./BAmp_GUI.cpp
	mkdir -p $(BUNDLE)
	mkdir -p $(BUNDLE)/tmp
	cd $(BUNDLE)/tmp; $(CC) $(CPPFLAGS) $(PUGLFLAGS) $(CFLAGS) $(GUICFLAGS) $(addprefix ../../, $(TKC)) -c
	cd $(BUNDLE)/tmp; $(CXX) $(CPPFLAGS) $(PUGLFLAGS) $(CXXFLAGS) $(GUICFLAGS) $(addprefix ../../, $< $(TKCXX)) -c
	$(CXX) $(CPPFLAGS) $(PUGLFLAGS) $(CXXFLAGS) $(LDFLAGS) $(GUICFLAGS) -Wl,--start-group $(GUILFLAGS) $(BUNDLE)/tmp/*.o -Wl,--end-group -o $(BUNDLE)/$@
	rm -rf $(BUNDLE)/tmp

install:
	mkdir -p $(DESTDIR)$(LV2DIR)
	rm -rf $(DESTDIR)$(LV2DIR)/$(BUNDLE)
	cp -R $(BUNDLE) $(DESTDIR)$(LV2DIR)

.PHONY: all

clean:
	rm -rf $(BUNDLE)
