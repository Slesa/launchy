#ifndef LINEEDITMENU_H
#define LINEEDITMENU_H


#include <QtGui/QLineEdit>


class LineEditMenu : public QLineEdit
{
	Q_OBJECT

public:
	LineEditMenu(QWidget* parent = 0);
	void contextMenuEvent(QContextMenuEvent *event);

signals:
	void menuEvent(QContextMenuEvent*);
};


#endif
