SHELL = /bin/sh

BUNDLE = BAmp.lv2
PREFIX ?= /usr/local
LV2DIR ?= $(PREFIX)/lib/lv2
PKG_CONFIG ?= pkg-config

CC ?= gcc
CXX ?= g++

CPPFLAGS += -DPIC
CFLAGS += -fvisibility=hidden -fPIC
CXXFLAGS += -std=c++17 -fvisibility=hidden -fPIC
LDFLAGS += -L$(CURDIR)/BWidgets/build -shared -pthread

DSPCFLAGS = `pkg-config --cflags --libs lv2`
GUICFLAGS = -I$(CURDIR)/BWidgets/include `$(PKG_CONFIG) --cflags lv2 x11 cairo`
DSPLIBS += -Wl,-Bstatic `$(PKG_CONFIG) --libs --static lv2` -Wl,-Bdynamic -lm
GUILIBS += -Wl,-Bstatic -lbwidgetscore -lcairoplus -lpugl -Wl,-Bdynamic `$(PKG_CONFIG) --libs lv2 cairo x11` -lm

$(BUNDLE): clean BAmp.so BAmp_GUI.so
	cp manifest.ttl BAmp.ttl $(BUNDLE)

all: $(BUNDLE)

BAmp.so: BAmp.cpp 
	mkdir -p $(BUNDLE)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(DSPCFLAGS) $< $(LDFLAGS) $(DSPLIBS) -o $(BUNDLE)/$@

BAmp_GUI.so: BAmp_GUI.cpp BWidgets/build
	mkdir -p $(BUNDLE)
	mkdir -p $(BUNDLE)/tmp
	cd $(BUNDLE)/tmp; $(CXX) $(CPPFLAGS) $(CXXFLAGS) $(GUICFLAGS) $(addprefix $(CURDIR)/, $<) -c
	$(CXX) $(BUNDLE)/tmp/*.o $(LDFLAGS) $(GUILIBS) -o $(BUNDLE)/$@
	rm -rf $(BUNDLE)/tmp

BWidgets/build:
	cd BWidgets ; $(MAKE) cairoplus CFLAGS+=-fvisibility=hidden
	cd BWidgets ; $(MAKE) pugl CPPFLAGS+="-DPIC -DPUGL_API=\"__attribute__((visibility(\\\"hidden\\\")))\"" CFLAGS+=-fvisibility=hidden
	cd BWidgets ; $(MAKE) bwidgets CXXFLAGS+=-fvisibility=hidden 

install:
	mkdir -p $(DESTDIR)$(LV2DIR)
	rm -rf $(DESTDIR)$(LV2DIR)/$(BUNDLE)
	cp -R $(BUNDLE) $(DESTDIR)$(LV2DIR)

.PHONY: all install clean

clean:
	rm -rf $(BUNDLE)
	cd BWidgets ; $(MAKE) clean
