
include ../flagsandlibs
include ../sources

xfwm4compositeeditor_SOURCES = $(COMMONSRC)

xfwm4compositeeditor_CFLAGS = -O3 -export-dynamic
xfwm4compositeeditor_CXXFLAGS = -O3 -export-dynamic

pixfilesdir = $(pkgdatadir)/pixmaps
pixfiles_DATA = ../resources/pixmaps/*

scriptfilesdir = $(pkgdatadir)/scripts
scriptfiles_SCRIPTS = ../resources/scripts/README

bin_PROGRAMS = xfwm4compositeeditor

man1_MANS = ../resources/man/xfwm4compositeeditor.1

install-exec-hook:
	cp ../resources/pixmaps/Xfwm4CompositeEditor.desktop /usr/share/applications/Xfwm4CompositeEditor.desktop
	echo Icon=${prefix}/share/Xfwm4CompositeEditor/pixmaps/Xfwm4CompositeEditor.png >> /usr/share/applications/Xfwm4CompositeEditor.desktop
	echo Exec=${prefix}/bin/xfwm4compositeeditor >> /usr/share/applications/Xfwm4CompositeEditor.desktop
#	update-menus

uninstall-hook:
	rm -f /usr/share/applications/Xfwm4CompositeEditor.desktop
#	update-menus
	rm -fr $(pkgdatadir)

