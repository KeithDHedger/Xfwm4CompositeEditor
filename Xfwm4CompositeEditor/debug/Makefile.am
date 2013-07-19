

include ../flagsandlibs
include ../sources

xfwm4compositeeditordebug_SOURCES = $(COMMONSRC)

xfwm4compositeeditordebug_CFLAGS = -ggdb -O0 -export-dynamic
xfwm4compositeeditordebug_CXXFLAGS = -ggdb -O0 -export-dynamic

bin_PROGRAMS = xfwm4compositeeditordebug

install-exec-local:
	cd $(DESTDIR)$(bindir) && \
	rm xfwm4compositeeditordebug

