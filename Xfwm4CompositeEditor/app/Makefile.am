TRANSFORMNAME = xfce4-composite-editor
APPNAME = xfce4-composite-editor
NAME = Xfwm4CompositeEditor

include ../flagsandlibs
include ../sources

xfwm4compositeeditor_SOURCES = $(COMMONSRC)

xfwm4compositeeditor_CFLAGS = -O3 -export-dynamic
xfwm4compositeeditor_CXXFLAGS = -O3 -export-dynamic

bin_PROGRAMS = xfwm4compositeeditor

docfilesdir = $(pkgdatadir)/docs
docfiles_DATA = ../resources/docs/*

install-exec-hook:
	mkdir -p $(DESTDIR)${prefix}/share/applications $(DESTDIR)${prefix}/share/pixmaps
	mv $(DESTDIR)${bindir}/xfwm4compositeeditor $(DESTDIR)${bindir}/$(TRANSFORMNAME)

	cp ../resources/pixmaps/$(NAME).desktop $(DESTDIR)${prefix}/share/applications/$(APPNAME).desktop
	cp ../resources/pixmaps/$(NAME).png $(DESTDIR)${prefix}/share/pixmaps

uninstall-hook:
	rm -f $(DESTDIR)${prefix}/share/applications/$(APPNAME).desktop
	rm -f $(DESTDIR)${prefix}/share/pixmaps/$(NAME).png
	rm -f $(DESTDIR)${bindir}/$(APPNAME)

