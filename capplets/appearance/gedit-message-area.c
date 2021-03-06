/*
 * gedit-message-area.c
 * This file is part of gedit
 *
 * Copyright (C) 2005 - Paolo Maggi
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
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330,
 * Boston, MA 02111-1307, USA.
 */

/*
 * Modified by the gedit Team, 2005. See the AUTHORS file for a
 * list of people on the gedit Team.
 * See the ChangeLog files for a list of changes.
 *
 * $Id: gedit-message-area.c 5080 2006-08-09 15:17:37Z pborelli $
 */

/* TODO: Style properties */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <glib/gi18n.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <gdk/gdkkeysyms.h>

#include "gedit-message-area.h"

#define GEDIT_MESSAGE_AREA_GET_PRIVATE(object)(G_TYPE_INSTANCE_GET_PRIVATE ((object), \
					       GEDIT_TYPE_MESSAGE_AREA, \
					       GeditMessageAreaPrivate))

struct _GeditMessageAreaPrivate
{
	GtkWidget *main_hbox;

	GtkWidget *contents;
	GtkWidget *action_area;

	gboolean changing_style;
};

typedef struct _ResponseData ResponseData;

struct _ResponseData
{
	gint response_id;
};

enum {
	RESPONSE,
	CLOSE,
	LAST_SIGNAL
};

static guint signals[LAST_SIGNAL];

G_DEFINE_TYPE(GeditMessageArea, gedit_message_area, GTK_TYPE_HBOX)


static void
gedit_message_area_finalize (GObject *object)
{
	/*
	GeditMessageArea *message_area = GEDIT_MESSAGE_AREA (object);
	*/

	G_OBJECT_CLASS (gedit_message_area_parent_class)->finalize (object);
}

static ResponseData *
get_response_data (GtkWidget *widget,
		   gboolean   create)
{
	ResponseData *ad = g_object_get_data (G_OBJECT (widget),
                                       	      "gedit-message-area-response-data");

	if (ad == NULL && create)
	{
		ad = g_new (ResponseData, 1);

		g_object_set_data_full (G_OBJECT (widget),
					"gedit-message-area-response-data",
					ad,
					g_free);
    	}

	return ad;
}

static GtkWidget *
find_button (GeditMessageArea *message_area,
	     gint              response_id)
{
	GList *children, *tmp_list;
	GtkWidget *child = NULL;

	children = gtk_container_get_children (
			GTK_CONTAINER (message_area->priv->action_area));

	for (tmp_list = children; tmp_list; tmp_list = tmp_list->next)
	{
		ResponseData *rd = get_response_data (tmp_list->data, FALSE);

		if (rd && rd->response_id == response_id)
		{
			child = tmp_list->data;
			break;
		}
	}

	g_list_free (children);

	return child;
}

static void
gedit_message_area_close (GeditMessageArea *message_area)
{
	if (!find_button (message_area, GTK_RESPONSE_CANCEL))
		return;

	/* emit response signal */
	gedit_message_area_response (GEDIT_MESSAGE_AREA (message_area),
				     GTK_RESPONSE_CANCEL);
}

static gboolean
paint_message_area (GtkWidget      *widget,
		    GdkEventExpose *event,
		    gpointer        user_data)
{
	gtk_paint_flat_box (widget->style,
			    widget->window,
			    GTK_STATE_NORMAL,
			    GTK_SHADOW_OUT,
			    NULL,
			    widget,
			    "tooltip",
			    widget->allocation.x,
			    widget->allocation.y,
			    widget->allocation.width,
			    widget->allocation.height);

	return FALSE;
}

static void
style_set (GtkWidget *widget,
	   GtkStyle  *prev_style)
{
	GtkTooltips *tooltips;
	GtkStyle *style;

	GeditMessageArea *message_area = GEDIT_MESSAGE_AREA (widget);

	if (message_area->priv->changing_style)
		return;

	tooltips = gtk_tooltips_new ();
	g_object_ref_sink (tooltips);

	gtk_tooltips_force_window (tooltips);

  if (GTK_IS_WIDGET (tooltips->tip_window)) {
		gtk_widget_ensure_style (tooltips->tip_window);
		style = gtk_widget_get_style (tooltips->tip_window);

  	message_area->priv->changing_style = TRUE;
  	gtk_widget_set_style (GTK_WIDGET (widget), style);
  	message_area->priv->changing_style = FALSE;
	}

	g_object_unref (tooltips);
}

static void
gedit_message_area_class_init (GeditMessageAreaClass *klass)
{
	GObjectClass *object_class;
	GtkWidgetClass *widget_class;
	GtkBindingSet *binding_set;

	object_class = G_OBJECT_CLASS (klass);
	widget_class = GTK_WIDGET_CLASS (klass);
	object_class->finalize = gedit_message_area_finalize;

	widget_class->style_set = style_set;

	klass->close = gedit_message_area_close;

	g_type_class_add_private (object_class, sizeof(GeditMessageAreaPrivate));

	signals[RESPONSE] = g_signal_new ("response",
					  G_OBJECT_CLASS_TYPE (klass),
					  G_SIGNAL_RUN_LAST,
					  G_STRUCT_OFFSET (GeditMessageAreaClass, response),
					  NULL, NULL,
					  g_cclosure_marshal_VOID__INT,
					  G_TYPE_NONE, 1,
					  G_TYPE_INT);

	signals[CLOSE] =  g_signal_new ("close",
					G_OBJECT_CLASS_TYPE (klass),
					G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION,
					G_STRUCT_OFFSET (GeditMessageAreaClass, close),
		  			NULL, NULL,
		  			g_cclosure_marshal_VOID__VOID,
					G_TYPE_NONE, 0);

	binding_set = gtk_binding_set_by_class (klass);

	gtk_binding_entry_add_signal (binding_set, GDK_Escape, 0, "close", 0);
}

static void
gedit_message_area_init (GeditMessageArea *message_area)
{
	message_area->priv = GEDIT_MESSAGE_AREA_GET_PRIVATE (message_area);

	message_area->priv->main_hbox = gtk_hbox_new (FALSE, 16); /* FIXME: use style properties */
	gtk_widget_show (message_area->priv->main_hbox);
	gtk_container_set_border_width (GTK_CONTAINER (message_area->priv->main_hbox),
					8); /* FIXME: use style properties */

	message_area->priv->action_area = gtk_vbox_new (TRUE, 10); /* FIXME: use style properties */
	gtk_widget_show (message_area->priv->action_area);
	gtk_box_pack_end (GTK_BOX (message_area->priv->main_hbox),
			    message_area->priv->action_area,
			    FALSE,
			    TRUE,
			    0);

	gtk_box_pack_start (GTK_BOX (message_area),
			    message_area->priv->main_hbox,
			    TRUE,
			    TRUE,
			    0);

	gtk_widget_set_app_paintable (GTK_WIDGET (message_area), TRUE);

	g_signal_connect (message_area,
			  "expose_event",
			  G_CALLBACK (paint_message_area),
			  NULL);
}

static gint
get_response_for_widget (GeditMessageArea *message_area,
			 GtkWidget        *widget)
{
	ResponseData *rd;

	rd = get_response_data (widget, FALSE);
	if (!rd)
		return GTK_RESPONSE_NONE;
	else
		return rd->response_id;
}

static void
action_widget_activated (GtkWidget *widget, GeditMessageArea *message_area)
{
	gint response_id;

	response_id = get_response_for_widget (message_area, widget);

	gedit_message_area_response (message_area, response_id);
}

void
gedit_message_area_add_action_widget (GeditMessageArea *message_area,
				      GtkWidget        *child,
				      gint              response_id)
{
	ResponseData *ad;
	guint signal_id;

	g_return_if_fail (GEDIT_IS_MESSAGE_AREA (message_area));
	g_return_if_fail (GTK_IS_WIDGET (child));

	ad = get_response_data (child, TRUE);

	ad->response_id = response_id;

	if (GTK_IS_BUTTON (child))
		signal_id = g_signal_lookup ("clicked", GTK_TYPE_BUTTON);
	else
		signal_id = GTK_WIDGET_GET_CLASS (child)->activate_signal;

	if (signal_id)
	{
		GClosure *closure;

		closure = g_cclosure_new_object (G_CALLBACK (action_widget_activated),
						 G_OBJECT (message_area));

		g_signal_connect_closure_by_id (child,
						signal_id,
						0,
						closure,
						FALSE);
	}
	else
		g_warning ("Only 'activatable' widgets can be packed into the action area of a GeditMessageArea");

	if (response_id != GTK_RESPONSE_HELP)
		gtk_box_pack_start (GTK_BOX (message_area->priv->action_area),
				    child,
				    FALSE,
				    FALSE,
				    0);
	else
		gtk_box_pack_end (GTK_BOX (message_area->priv->action_area),
				    child,
				    FALSE,
				    FALSE,
				    0);
}

void
gedit_message_area_set_contents	(GeditMessageArea *message_area,
				 GtkWidget        *contents)
{
	g_return_if_fail (GEDIT_IS_MESSAGE_AREA (message_area));
	g_return_if_fail (GTK_IS_WIDGET (contents));

  	message_area->priv->contents = contents;
	gtk_box_pack_start (GTK_BOX (message_area->priv->main_hbox),
			    message_area->priv->contents,
			    TRUE,
			    TRUE,
			    0);
}

GtkWidget*
gedit_message_area_add_button (GeditMessageArea *message_area,
			       const gchar      *button_text,
			       gint              response_id)
{
	GtkWidget *button;

	g_return_val_if_fail (GEDIT_IS_MESSAGE_AREA (message_area), NULL);
	g_return_val_if_fail (button_text != NULL, NULL);

	button = gtk_button_new_from_stock (button_text);

	GTK_WIDGET_SET_FLAGS (button, GTK_CAN_DEFAULT);

	gtk_widget_show (button);

	gedit_message_area_add_action_widget (message_area,
					      button,
					      response_id);

	return button;
}

static void
add_buttons_valist (GeditMessageArea *message_area,
		    const gchar      *first_button_text,
		    va_list           args)
{
	const gchar* text;
	gint response_id;

	g_return_if_fail (GEDIT_IS_MESSAGE_AREA (message_area));

	if (first_button_text == NULL)
		return;

	text = first_button_text;
	response_id = va_arg (args, gint);

	while (text != NULL)
	{
		gedit_message_area_add_button (message_area,
					       text,
					       response_id);

		text = va_arg (args, gchar*);
		if (text == NULL)
        		break;

		response_id = va_arg (args, int);
	}
}

void
gedit_message_area_add_buttons (GeditMessageArea *message_area,
				const gchar      *first_button_text,
				...)
{
	va_list args;

	va_start (args, first_button_text);

	add_buttons_valist (message_area,
                            first_button_text,
                            args);

	va_end (args);
}

GtkWidget *
gedit_message_area_new (void)
{
	return g_object_new (GEDIT_TYPE_MESSAGE_AREA, NULL);
}

GtkWidget *
gedit_message_area_new_with_buttons (const gchar *first_button_text,
                                     ...)
{
	GeditMessageArea *message_area;
	va_list args;

	message_area = GEDIT_MESSAGE_AREA (gedit_message_area_new ());

	va_start (args, first_button_text);

	add_buttons_valist (message_area,
			    first_button_text,
			    args);

	va_end (args);

	return GTK_WIDGET (message_area);
}

void
gedit_message_area_set_response_sensitive (GeditMessageArea *message_area,
					   gint              response_id,
					   gboolean          setting)
{
	GList *children;
	GList *tmp_list;

	g_return_if_fail (GEDIT_IS_MESSAGE_AREA (message_area));

	children = gtk_container_get_children (GTK_CONTAINER (message_area->priv->action_area));

	tmp_list = children;
	while (tmp_list != NULL)
	{
		GtkWidget *widget = tmp_list->data;
		ResponseData *rd = get_response_data (widget, FALSE);

		if (rd && rd->response_id == response_id)
			gtk_widget_set_sensitive (widget, setting);

		tmp_list = g_list_next (tmp_list);
	}

	g_list_free (children);
}

void
gedit_message_area_set_default_response (GeditMessageArea *message_area,
					 gint              response_id)
{
	GList *children;
	GList *tmp_list;

	g_return_if_fail (GEDIT_IS_MESSAGE_AREA (message_area));

	children = gtk_container_get_children (GTK_CONTAINER (message_area->priv->action_area));

	tmp_list = children;
	while (tmp_list != NULL)
	{
		GtkWidget *widget = tmp_list->data;
		ResponseData *rd = get_response_data (widget, FALSE);

		if (rd && rd->response_id == response_id)
		gtk_widget_grab_default (widget);

		tmp_list = g_list_next (tmp_list);
	}

	g_list_free (children);
}

void
gedit_message_area_response (GeditMessageArea *message_area,
			     gint              response_id)
{
	g_return_if_fail (GEDIT_IS_MESSAGE_AREA (message_area));

	g_signal_emit (message_area,
		       signals[RESPONSE],
		       0,
		       response_id);
}

GtkWidget *
gedit_message_area_add_stock_button_with_text (GeditMessageArea *message_area,
				    	       const gchar      *text,
				    	       const gchar      *stock_id,
				    	       gint              response_id)
{
	GtkWidget *button;

	g_return_val_if_fail (GEDIT_IS_MESSAGE_AREA (message_area), NULL);
	g_return_val_if_fail (text != NULL, NULL);
	g_return_val_if_fail (stock_id != NULL, NULL);

	button = gtk_button_new_with_mnemonic (text);
        gtk_button_set_image (GTK_BUTTON (button),
                              gtk_image_new_from_stock (stock_id,
                                                        GTK_ICON_SIZE_BUTTON));

	GTK_WIDGET_SET_FLAGS (button, GTK_CAN_DEFAULT);

	gtk_widget_show (button);

	gedit_message_area_add_action_widget (message_area,
					      button,
					      response_id);

	return button;
}

