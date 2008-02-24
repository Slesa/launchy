#ifndef PLATFORM_GNOME_UTIL_H
#define PLATFORM_GNOME_UTIL_H


#include <QFileIconProvider>

#include <QWidget>



class GnomeAlphaBorder : public QWidget
{
    Q_OBJECT    
private:
    public:
QString alphaFile;
    void ShowAlpha(QString filename);
    GnomeAlphaBorder(QWidget * parent);
    void paintEvent(QPaintEvent *);
};

class GnomeIconProvider : QFileIconProvider
{
 public:
    GnomeIconProvider() {}
    ~GnomeIconProvider() {}
    QIcon icon(const QFileInfo& info) const;
};


#endif
