# This is used in GNOMECC_CAPPLETS_CFLAGS
cappletname = window

bin_PROGRAMS = gnome-window-properties

gnome_window_properties_LDADD = $(GNOMECC_CAPPLETS_LIBS) 	\
	$(top_builddir)/libwindow-settings/libgnome-window-settings.la

gnome_window_properties_SOURCES = 		\
	gnome-window-properties.c

@INTLTOOL_DESKTOP_RULE@

gladedir   = $(pkgdatadir)/glade
glade_DATA = gnome-window-properties.glade

desktopdir = $(datadir)/applications
Desktop_in_files = window-properties.desktop.in
desktop_DATA = $(Desktop_in_files:.desktop.in=.desktop)

INCLUDES   = $(GNOMECC_CAPPLETS_CFLAGS)					\
	     -I$(top_srcdir)/libwindow-settings \
	     -DGNOME_WINDOW_MANAGER_MODULE_PATH=\""$(libdir)/window-manager-settings"\" \
	     -DGLADEDIR=\""$(gladedir)"\"  	\
	     -DGNOMELOCALEDIR="\"$(datadir)/locale\""			\
	     -DPIXMAPDIR=\""$(pixmapdir)"\"

CLEANFILES = $(GNOMECC_CAPPLETS_CLEANFILES) $(Desktop_in_files) $(desktop_DATA)
EXTRA_DIST = $(glade_DATA)

