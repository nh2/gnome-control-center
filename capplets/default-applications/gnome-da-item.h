/*
 *  Authors: Luca Cavalli <loopback@slackit.org>
 *
 *  Copyright 2005-2006 Luca Cavalli
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of version 2 of the GNU General Public License
 *  as published by the Free Software Foundation
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Street #330, Boston, MA 02111-1307, USA.
 *
 */

#ifndef _GNOME_DA_ITEM_H_
#define _GNOME_DA_ITEM_H_

#include <glib.h>

typedef struct _GnomeDAItem GnomeDAItem;

typedef struct _GnomeDAWebItem GnomeDAWebItem;
typedef struct _GnomeDAMailItem GnomeDAMailItem;
typedef struct _GnomeDATermItem GnomeDATermItem;

struct _GnomeDAItem {
    gchar *name;
    gchar *executable;
    gchar *command;
    gchar *icon_name;
};

struct _GnomeDAWebItem {
    GnomeDAItem generic;
    gboolean run_in_terminal;
    gboolean netscape_remote;
    gchar *tab_command;
    gchar *win_command;
};

struct _GnomeDAMailItem {
    GnomeDAItem generic;
    gboolean run_in_terminal;
};

struct _GnomeDATermItem {
    GnomeDAItem generic;
    gchar *exec_flag;
};

GnomeDAWebItem* gnome_da_web_item_new (void);
GnomeDAMailItem* gnome_da_mail_item_new (void);
GnomeDATermItem* gnome_da_term_item_new (void);
void gnome_da_web_item_free (GnomeDAWebItem *item);
void gnome_da_mail_item_free (GnomeDAMailItem *item);
void gnome_da_term_item_free (GnomeDATermItem *item);

#endif