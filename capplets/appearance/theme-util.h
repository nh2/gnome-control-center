/*
 * Copyright (C) 2007 The GNOME Foundation
 * Written by Jens Granseuer <jensgr@gmx.net>
 * All Rights Reserved
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#define GTK_THEME_KEY "/desktop/gnome/interface/gtk_theme"
#define METACITY_THEME_KEY "/apps/metacity/general/theme"
#define ICON_THEME_KEY "/desktop/gnome/interface/icon_theme"
#define COLOR_SCHEME_KEY "/desktop/gnome/interface/gtk_color_scheme"
#define CURSOR_THEME_KEY "/desktop/gnome/peripherals/mouse/cursor_theme"
#define LOCKDOWN_KEY "/desktop/gnome/lockdown/disable_theme_settings"

enum {
  COL_THUMBNAIL,
  COL_LABEL,
  COL_NAME,
  NUM_COLS
};

typedef enum {
  THEME_TYPE_GTK,
  THEME_TYPE_WINDOW,
  THEME_TYPE_ICON,
  THEME_TYPE_META,
  THEME_TYPE_CURSOR
} ThemeType;

gboolean theme_delete (const gchar *name, ThemeType type);
gboolean theme_model_iter_last (GtkTreeModel *model, GtkTreeIter *iter);