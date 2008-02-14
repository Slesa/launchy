#ifndef PLATFORM_GNOME_UTIL_H
#define PLATFORM_GNOME_UTIL_H

#include <QFileIconProvider>

class GnomeIconProvider : QFileIconProvider
{
 public:
    GnomeIconProvider() {}
    ~GnomeIconProvider() {}
    QIcon icon(const QFileInfo& info) const;
};


#endif
