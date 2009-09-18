#include "precompiled.h"
#include "DropListWidget.h"


DropListWidget::DropListWidget(QWidget* pParent) :
	QListWidget(pParent)
{
	setAcceptDrops(true);
}


DropListWidget::~DropListWidget()
{
}


void DropListWidget::dragEnterEvent(QDragEnterEvent *event)
{
	emit dragEnter(event);
}


void DropListWidget::dragMoveEvent(QDragMoveEvent *event)
{
	emit dragMove(event);
}


void DropListWidget::dropEvent(QDropEvent *event)
{
	emit drop(event);
}
