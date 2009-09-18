#include "precompiled.h"
#include "CharListWidget.h"


CharListWidget::CharListWidget(QWidget* parent) : 
	QListWidget(parent)
{
#ifdef Q_WS_X11
	setWindowFlags( windowFlags() | Qt::Tool | Qt::SplashScreen);
#endif
	setAttribute(Qt::WA_AlwaysShowToolTips);

	setAlternatingRowColors(true);
}


void CharListWidget::keyPressEvent(QKeyEvent* key)
{
	emit keyPressed(key);
	QListWidget::keyPressEvent(key);
	key->ignore();
}


void CharListWidget::mouseDoubleClickEvent(QMouseEvent* /*event*/)
{
	QKeyEvent key(QEvent::KeyPress, Qt::Key_Enter, NULL);
	emit keyPressed(&key);
}


void CharListWidget::focusOutEvent(QFocusEvent* event)
{
	emit focusOut(event);
}
