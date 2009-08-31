#include "LineEditMenu.h"


LineEditMenu::LineEditMenu(QWidget* parent) :
	QLineEdit(parent)
{
	setAttribute(Qt::WA_InputMethodEnabled);
}


void LineEditMenu::contextMenuEvent(QContextMenuEvent *evt)
{
	emit menuEvent(evt);
}
