## Process this file with automake to produce Makefile.in 

SUBDIRS = CSeries Files Videos GameWorld Input Lua Misc	\
   ModelView Network RenderMain RenderOther Sound TCPMess XML

if STANDALONE_HUB
bin_PROGRAMS = standalone_hub
else
if MAKE_WINDOWS
bin_PROGRAMS = AlephOne Marathon Marathon2 MarathonInfinity
else
bin_PROGRAMS = alephone
if BUILD_ALEPHONE_TESTS
bin_PROGRAMS += alephone_tests
endif
endif
endif

alephone_SOURCES = shell.h \
  \
  shell.cpp shell_misc.cpp shell_options.h shell_options.cpp main.cpp \
  $(top_srcdir)/VitaPlatform/vita_platform.h $(top_srcdir)/VitaPlatform/vita_platform.c \
  $(top_srcdir)/VitaPlatform/vita_adapter.cpp $(top_srcdir)/VitaPlatform/vita_sound.h \
  $(top_srcdir)/VitaPlatform/vita_sound.cpp $(top_srcdir)/VitaPlatform/vita_alephone_config.h

if HOST_VITA
# Vitasdk root; overridable from the environment (export VITASDK=...).
VITASDK ?= /usr/local/vitasdk
VITA_LIBDIR ?= $(VITASDK)/arm-vita-eabi/lib
# Use the project's Vita startup object.  It initializes newlib before C++
# global constructors; the SDK crt0 in some toolchain bundles omits that step.
VITA_CRT_DIR = $(abs_top_builddir)/vita-crt
VITA_OPENAL_LIB = $(VITA_LIBDIR)/libopenal.a

$(VITA_CRT_DIR)/crt0.o: $(top_srcdir)/VitaPlatform/crt0.c
	$(MKDIR_P) $(VITA_CRT_DIR)
	$(CC) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CFLAGS) $(CFLAGS) -c $< -o $@

alephone_DEPENDENCIES = $(VITA_CRT_DIR)/crt0.o

# The Vitasdk openal package exposes a global EffectList that collides with
# Aleph One's GameWorld::EffectList (Source_Files/GameWorld/effects.h). Localize
# openal's private copy (used only within alEffect.c) so both coexist.
vita-localize-openal.stamp: $(VITA_OPENAL_LIB)
	@if test -f "$<"; then \
	  echo "  OBJCOPY  localize openal EffectList"; \
	  arm-vita-eabi-objcopy --localize-symbol=EffectList "$<" && touch $@; \
	else \
	  echo "vita-localize-openal: missing $<" >&2; false; \
	fi

BUILT_SOURCES = vita-localize-openal.stamp
EXTRA_alephone_DEPENDENCIES = vita-localize-openal.stamp
endif

alephone_LDADD = \
  CSeries/libcseries.a Files/libfiles.a Videos/libvideos.a \
  GameWorld/libgameworld.a Input/libinput.a Lua/liba1lua.a Misc/libmisc.a \
  ModelView/libmodelview.a Network/libnetwork.a Network/Metaserver/libmetaserver.a \
  RenderMain/librendermain.a RenderOther/librenderother.a Sound/libsound.a \
  XML/libxml.a \
  \
  CSeries/libcseries.a Files/libfiles.a Videos/libvideos.a \
  GameWorld/libgameworld.a Input/libinput.a Lua/liba1lua.a Misc/libmisc.a \
  ModelView/libmodelview.a Network/libnetwork.a Network/Metaserver/libmetaserver.a \
  RenderMain/librendermain.a RenderOther/librenderother.a Sound/libsound.a \
  TCPMess/libtcpmess.a XML/libxml.a \
  $(NULL)

standalone_hub_SOURCES = shell.h shell.cpp shell_misc.cpp shell_options.h shell_options.cpp Network/StandaloneHub/standalone_hub_main.cpp

standalone_hub_LDADD = $(alephone_LDADD) Network/StandaloneHub/libstandalonehub.a

alephone_tests_SOURCES = shell.h shell.cpp shell_misc.cpp shell_options.h shell_options.cpp $(top_srcdir)/tests/replay_film_test.cpp $(top_srcdir)/tests/main.cpp
alephone_tests_LDADD = $(alephone_LDADD)

AM_CPPFLAGS = -I$(top_srcdir)/Source_Files/CSeries -I$(top_srcdir)/Source_Files/Files \
  -I$(top_srcdir)/Source_Files/GameWorld -I$(top_srcdir)/Source_Files/Input \
  -I$(top_srcdir)/Source_Files/Lua -I$(top_srcdir)/Source_Files/Misc \
  -I$(top_srcdir)/Source_Files/ModelView -I$(top_srcdir)/Source_Files/Network \
  -I$(top_srcdir)/Source_Files/Network/Metaserver \
  -I$(top_srcdir)/Source_Files/Videos -I$(top_srcdir)/Source_Files/RenderMain \
  -I$(top_srcdir)/Source_Files/RenderOther -I$(top_srcdir)/Source_Files/Sound \
  -I$(top_srcdir)/Source_Files/XML -I$(top_srcdir)/Source_Files/TCPMess \
  -I$(top_srcdir)/VitaPlatform

if HOST_VITA
AM_CPPFLAGS += -DDISABLE_NETWORKING
# Keep relocation records in the final ARM ELF. vita-elf-create converts them
# to SCE relocations so the Vita loader can independently move the RX and RW
# segments without corrupting C++ init arrays and other absolute pointers.
alephone_LDFLAGS = -B$(VITA_CRT_DIR)/ -Wl,-q
endif

AlephOne_LDADD = $(alephone_LDADD) alephone-resources.o
AlephOne_SOURCES = $(alephone_SOURCES)

Marathon_LDADD = $(alephone_LDADD) marathon-resources.o
Marathon_SOURCES = $(alephone_SOURCES)

Marathon2_LDADD = $(alephone_LDADD) marathon2-resources.o
Marathon2_SOURCES = $(alephone_SOURCES)

MarathonInfinity_LDADD = $(alephone_LDADD) marathon-infinity-resources.o
MarathonInfinity_SOURCES = $(alephone_SOURCES)

if MAKE_WINDOWS
BUILD_YEAR = `echo $(VERSION) | cut -c 1-4`
BUILD_MONTH = `echo $(VERSION) | cut -c 5-6 | sed -e s/^0//`
BUILD_DAY = `echo $(VERSION) | cut -c 7-8 | sed -e s/^0//`
WIN_VERSION_STRING= 0, $(BUILD_YEAR), $(BUILD_MONTH), $(BUILD_DAY)
A1_DISPLAY_VERSION= `grep '^\#define A1_DISPLAY_VERSION' $(srcdir)/Misc/alephversion.h | sed -e 's/\(.*\"\)\(.*\)\(\"\)/\2/g' | tr -d '\n'`
endif

alephone-resources.o: $(top_srcdir)/Resources/Windows/alephone.rc Misc/alephversion.h
	sed -e "s/WIN_VERSION_STRING/$(WIN_VERSION_STRING)/g" -e "s/A1_DISPLAY_VERSION/$(A1_DISPLAY_VERSION)/g" $(top_srcdir)/Resources/Windows/alephone.rc | @WINDRES@ -I$(top_srcdir)/Resources/Windows -o alephone-resources.o

marathon-resources.o: $(top_srcdir)/Resources/Windows/marathon.rc Misc/alephversion.h $(top_srcdir)/Resources/Windows/marathon.ico
	sed -e "s/WIN_VERSION_STRING/$(WIN_VERSION_STRING)/g" -e "s/A1_DISPLAY_VERSION/$(A1_DISPLAY_VERSION)/g" $(top_srcdir)/Resources/Windows/marathon.rc | @WINDRES@ -I$(top_srcdir)/Resources/Windows -o marathon-resources.o

marathon2-resources.o: $(top_srcdir)/Resources/Windows/marathon2.rc Misc/alephversion.h $(top_srcdir)/Resources/Windows/marathon2.ico
	sed -e "s/WIN_VERSION_STRING/$(WIN_VERSION_STRING)/g" -e "s/A1_DISPLAY_VERSION/$(A1_DISPLAY_VERSION)/g" $(top_srcdir)/Resources/Windows/marathon2.rc | @WINDRES@ -I$(top_srcdir)/Resources/Windows -o marathon2-resources.o

marathon-infinity-resources.o: $(top_srcdir)/Resources/Windows/marathon-infinity.rc Misc/alephversion.h $(top_srcdir)/Resources/Windows/marathon-infinity.ico
	sed -e "s/WIN_VERSION_STRING/$(WIN_VERSION_STRING)/g" -e "s/A1_DISPLAY_VERSION/$(A1_DISPLAY_VERSION)/g" $(top_srcdir)/Resources/Windows/marathon-infinity.rc | @WINDRES@ -I$(top_srcdir)/Resources/Windows -o marathon-infinity-resources.o
