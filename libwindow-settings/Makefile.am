WM_MODULE_DIR=$(libdir)/window-manager-settings

INCLUDES = 								\
	-DGNOMELOCALEDIR="\"$(datadir)/locale\""			\
	-DGNOME_ICONDIR=\""$(datadir)/pixmaps"\"			\
	-DG_LOG_DOMAIN=\"capplet-common\"				\
	-DGNOME_WM_PROPERTY_PATH=\"$(datadir)/gnome/wm-properties\"	\
	-DGNOME_WINDOW_MANAGER_MODULE_PATH=\""$(WM_MODULE_DIR)"\"	\
	-I$(top_srcdir)/						\
	@CAPPLET_CFLAGS@						\
	-DMETACITY_THEME_DIR=\""$(datadir)/themes"\"

lib_LTLIBRARIES = libgnome-window-settings.la

libgnome_window_settings_la_LDFLAGS = 	\
	-export_dynamic			\
	-no-undefined			\
        -version-info 1:0:0

libgnome_window_settings_la_LIBADD = $(GNOMECC_LIBS)

libgnome_window_settings_la_SOURCES = \
	gnome-window-manager.c	\
	gnome-window-manager.h	\
	gnome-wm-manager.c	\
	gnome-wm-manager.h

libgnome_window_settingsincludedir = $(includedir)/gnome-window-settings-2.0

libgnome_window_settingsinclude_HEADERS =	\
	gnome-window-manager.h			\
	gnome-wm-manager.h

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = gnome-window-settings-2.0.pc

EXTRA_DIST = 					\
	gnome-window-settings-2.0.pc.in		\
	ChangeLog

wms_flags = -export_dynamic -avoid-version -module
wmsdir = $(WM_MODULE_DIR)

wms_LTLIBRARIES =			\
	libmetacity.la

libmetacity_la_SOURCES = 		\
	metacity-window-manager.c	\
	metacity-window-manager.h

libmetacity_la_LDFLAGS = $(wms_flags)
libmetacity_la_LIBADD = 		\
	$(GNOMECC_LIBS)			\
	libgnome-window-settings.la
