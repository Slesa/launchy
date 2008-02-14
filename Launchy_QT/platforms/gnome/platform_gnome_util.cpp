#include <libgnomeui/libgnomeui.h>

#include "platform_gnome_util.h"
#include <QPixmap>
#include <QIcon>
#include <QDebug>

QIcon GnomeIconProvider::icon(const QFileInfo& info) const
{
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
