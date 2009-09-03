#ifndef CHARLINEEDIT_H
#define CHARLINEEDIT_H


#include <QtGui/QLineEdit>


class CharLineEdit : public QLineEdit
{
	Q_OBJECT

public:
	CharLineEdit(QWidget* parent = 0);

	void keyPressEvent(QKeyEvent* key);
	bool focusNextPrevChild(bool next);
	void focusOutEvent(QFocusEvent* event);
	void inputMethodEvent(QInputMethodEvent *e);
	QString separatorText() const;

signals:
	void keyPressed(QKeyEvent*);
	void focusOut(QFocusEvent* evt);
	void inputMethod(QInputMethodEvent *e);

private:
	QChar sepChar() const;
};


#endif
