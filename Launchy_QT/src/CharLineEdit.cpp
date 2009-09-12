#include <QKeyEvent>
#include "CharLineEdit.h"


CharLineEdit::CharLineEdit(QWidget* parent) :
	QLineEdit(parent)
{
	setAttribute(Qt::WA_InputMethodEnabled);
}


void CharLineEdit::keyPressEvent(QKeyEvent* event)
{
	bool handled = false;

	if (selectionStart() == -1)
	{
		switch (event->key())
		{
		case Qt::Key_Backspace:
			if (isAtEndOfSeparator())
			{
				// Delete separator characters in a single keypress.
				// Don't use setText This method maintains the undo history
				backspace();
				backspace();
				backspace();
				handled = true;
			}
			break;

		case Qt::Key_Delete:
			if (isAtStartOfSeparator())
			{
				del();
				del();
				del();
				handled = true;
			}
			break;

		case Qt::Key_Left:
			if (isAtEndOfSeparator())
			{
				cursorBackward(false, 3);
				handled = true;
			}
			break;

		case Qt::Key_Right:
			if (isAtStartOfSeparator())
			{
				cursorForward(false, 3);
				handled = true;
			}
			break;
		}
	}

	if (handled)
	{
		event->ignore();
	}
	else
	{
		QLineEdit::keyPressEvent(event);
	}

	emit keyPressed(event);
}


// This is how you pick up the tab key
bool CharLineEdit::focusNextPrevChild(bool next)
{
	next; // Remove compiler warning
	QKeyEvent event(QEvent::KeyPress, Qt::Key_Tab, NULL);
	emit keyPressed(&event);
	
	return true;
}


void CharLineEdit::focusInEvent(QFocusEvent* event)
{
	QLineEdit::focusInEvent(event);

	emit focusIn(event);
}


void CharLineEdit::focusOutEvent(QFocusEvent* event)
{
	QLineEdit::focusOutEvent(event);

	emit focusOut(event);
}


void CharLineEdit::inputMethodEvent(QInputMethodEvent* event)
{
	QLineEdit::inputMethodEvent(event);

	if (event->commitString() != "")
	{
		emit inputMethod(event);
	}
}


QChar CharLineEdit::separatorChar() const
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
	return QString(" ") + separatorChar() + " ";
}


bool CharLineEdit::isAtStartOfSeparator() const
{
	return text().mid(cursorPosition(), 3) == separatorText();
}


bool CharLineEdit::isAtEndOfSeparator() const
{
	return text().mid(cursorPosition() - 3, 3) == separatorText();
}
