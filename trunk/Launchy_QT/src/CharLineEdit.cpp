#include <QKeyEvent>
#include "CharLineEdit.h"


CharLineEdit::CharLineEdit(QWidget* parent) :
	QLineEdit(parent)
{
	setAttribute(Qt::WA_InputMethodEnabled);
}


void CharLineEdit::keyPressEvent(QKeyEvent* key)
{
	if (key->key() == Qt::Key_Backspace && selectionStart() == -1)
	{
		int cursor = cursorPosition() - 3;
		if (text().mid(cursor, 3) == separatorText())
		{
			// Delete separator characters in a single keypress
			setText(text().left(cursor) + text().mid(cursor+3));
			setCursorPosition(cursor);
			key->ignore();
		}
		else
		{
			QLineEdit::keyPressEvent(key);
		}
	}
	else
	{
		QLineEdit::keyPressEvent(key);
	}

	emit keyPressed(key);
}


// This is how you pick up the tab key
bool CharLineEdit::focusNextPrevChild(bool next)
{
	next = next; // Remove compiler warning
	QKeyEvent key(QEvent::KeyPress, Qt::Key_Tab, NULL);
	emit keyPressed(&key);
	return true;
}


void CharLineEdit::focusOutEvent(QFocusEvent* event)
{
	emit focusOut(event);
}


void CharLineEdit::inputMethodEvent(QInputMethodEvent *e)
{
	QLineEdit::inputMethodEvent(e);
	if (e->commitString() != "")
	{
		emit inputMethod(e);
	}
}


QChar CharLineEdit::sepChar() const
{
	QFontMetrics met = fontMetrics();
	QChar arrow(0x25ba);
	if (met.inFont(arrow))
		return arrow;
	else
		return QChar('|');
}


QString CharLineEdit::separatorText() const
{
	return QString(" ") + sepChar() + " ";
}
