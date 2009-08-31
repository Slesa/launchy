#ifndef CHARLISTWIDGET_H
#define CHARLISTWIDGET_H


#include <QtGui/QListWidget>


class CharListWidget : public QListWidget
{
	Q_OBJECT

public:
	CharListWidget(QWidget* parent = 0);
	void keyPressEvent(QKeyEvent* key);
	void mouseDoubleClickEvent(QMouseEvent* event);
	void focusOutEvent(QFocusEvent* event);

signals:
	void keyPressed(QKeyEvent*);
	void focusOut(QFocusEvent* event);
};


#endif
