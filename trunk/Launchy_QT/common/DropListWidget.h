#ifndef DROPLISTWIDGET_H
#define DROPLISTWIDGET_H


#include <QtGui/QListWidget>


class DropListWidget : public QListWidget
{
   Q_OBJECT

public:

	DropListWidget(QWidget* pParent = NULL);
   ~DropListWidget();

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);

signals:
    void dragEnter(QDragEnterEvent *event);
    void dragMove(QDragMoveEvent *event);
    void drop(QDropEvent *event);
};


#endif
