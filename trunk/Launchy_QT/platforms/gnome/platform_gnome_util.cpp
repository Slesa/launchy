#include <libgnomeui/libgnomeui.h>


#include "platform_gnome_util.h"
#include <gtk/gtk.h>
#include <gdk/gdkscreen.h>
#include <cairo.h>

#include <QPixmap>
#include <QIcon>
#include <QDebug>

/* Only some X servers support alpha channels. Always have a fallback */
gboolean supports_alpha = FALSE;

GtkWidget* gtk_win = NULL;


void alphaScreenChanged(GtkWidget *widget, GdkScreen *old_screen, QString alphaFile)
{
    /* To check if the display supports alpha channels, get the colormap */
    GdkScreen *screen = gtk_widget_get_screen(widget);
    GdkColormap *colormap = gdk_screen_get_rgba_colormap(screen);

    if (!colormap)
    {
        printf("Your screen does not support alpha channels!\n");
        colormap = gdk_screen_get_rgb_colormap(screen);
        supports_alpha = FALSE;
    }
    else
    {
        printf("Your screen supports alpha channels!\n");
        supports_alpha = TRUE;
    }

    /* Now we have a colormap appropriate for the screen, use it */
    gtk_widget_set_colormap(widget, colormap);
}

/* This is called when we need to draw the windows contents */
bool alphaExpose(GtkWidget *widget, GdkEventExpose *event, QString alphaFile)
{
    cairo_t *cr = gdk_cairo_create(widget->window);

    if (!supports_alpha)
	return FALSE;
    
    cairo_set_source_rgba (cr, 1.0, 1.0, 1.0, 0.0); /* transparent */
    
    /* draw the background */
    cairo_set_operator (cr, CAIRO_OPERATOR_SOURCE);
    cairo_paint (cr);


    /* draw a circle */

    //int width, height;
    //gtk_window_get_size(GTK_WINDOW(widget), &width, &height);

    //cairo_set_source_rgba(cr, 1, 0.2, 0.2, 0.6);
    //    cairo_arc(cr, width / 2, height / 2, (width < height ? width : height) / 2 - 8 , 0, 2 * 3.14);
    //cairo_fill(cr);
    //cairo_stroke(cr);
    

    cairo_surface_t* pCairoSurface = cairo_image_surface_create_from_png(alphaFile.toLocal8Bit().data());
    cairo_set_source_surface(cr, pCairoSurface,0,0);
    cairo_mask_surface(cr, pCairoSurface, 0,0);

    cairo_destroy(cr);
    return FALSE;
}

GnomeAlphaBorder::GnomeAlphaBorder(QWidget * parent) : 
    QWidget(parent, Qt::Tool | Qt::FramelessWindowHint)
{
    resize(parent->size());
    gtk_win  = gtk_plug_new(winId());
    gtk_widget_set_app_paintable(gtk_win, TRUE);
    gtk_window_set_decorated(GTK_WINDOW(gtk_win), FALSE);
    gtk_widget_show_all(gtk_win);
}

void GnomeAlphaBorder::paintEvent(QPaintEvent *)
{
    qDebug() << "calling expose";
    alphaExpose(gtk_win, NULL, alphaFile);
}

void GnomeAlphaBorder::ShowAlpha(QString file)
{
    alphaFile = file;
    qDebug() << alphaFile;
    alphaScreenChanged(gtk_win, NULL, alphaFile);
}

QIcon GnomeIconProvider::icon(const QFileInfo& info) const
{
    if (info.fileName().endsWith(".png", Qt::CaseInsensitive))
	{
	    return QIcon(info.absoluteFilePath());
	}
    GnomeIconLookupResultFlags resultFlags;
    char* file = gnome_icon_lookup_sync(gtk_icon_theme_get_default(),
					NULL,
					info.absoluteFilePath().toLocal8Bit().data(),
					NULL,
					GNOME_ICON_LOOKUP_FLAGS_NONE,
					&resultFlags);
    GtkIconInfo* icinfo = gtk_icon_theme_lookup_icon(gtk_icon_theme_get_default(),
			       file,
			       32,
			       GTK_ICON_LOOKUP_NO_SVG);
    
    GdkPixbuf* buff = gtk_icon_info_load_icon(icinfo, NULL);
    gchar* pixmap;
    gsize buflen;

    gdk_pixbuf_save_to_buffer (buff,
			       &pixmap,
			       &buflen,
			       "png",
			       NULL, NULL);
    QPixmap qp;
    qp.loadFromData((const uchar*) pixmap, buflen, "png");
    QIcon qico(qp);
    free(pixmap);
    g_object_unref(buff);

    return qp;
}
