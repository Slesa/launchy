#ifndef CHARLINEEDIT_H
#define CHARLINEEDIT_H


#include <QtGui/QLineEdit>


class CharLineEdit : public QLineEdit
{
	Q_OBJECT

public:
	CharLineEdit(QWidget* parent = 0);

	void keyPressEvent(QKeyEvent* event);
	bool focusNextPrevChild(bool next);
    void focusInEvent(QFocusEvent* event);
	void focusOutEvent(QFocusEvent* event);
	void inputMethodEvent(QInputMethodEvent *event);
	QString separatorText() const;

signals:
	void keyPressed(QKeyEvent*);
	void focusIn(QFocusEvent* evt);
	void focusOut(QFocusEvent* evt);
	void inputMethod(QInputMethodEvent *e);

private:
	bool isAtStartOfSeparator() const;
	bool isAtEndOfSeparator() const;
	QChar separatorChar() const;
};


#endif
