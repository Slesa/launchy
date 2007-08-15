#ifndef OPTIONS_H
#define OPTIONS_H

#include "ui_options.h"
#include <QString>
#include <QStringList>
#include <QList>

struct Directory {
	Directory() 
		: indexDirs(false), indexExe(false), depth(100)
	{
	
	}
	bool indexDirs;
	bool indexExe;
	QString name;
	QStringList types;
	int depth;
};

class OptionsDlg : public QDialog, private Ui::OptionsDlg
{
  Q_OBJECT

public:

QStringList metaKeys;
QStringList actionKeys;
QList<int> iMetaKeys;
QList<int> iActionKeys;
OptionsDlg(QWidget* parent = 0);
QList<Directory> memDirs;

public slots:
	void accept();
	void reject();
	void skinChanged(const QString);
	void dirChanged(int row);
	void catDirPlusClicked(bool c);
	void catDirMinusClicked(bool c);
	void catTypesPlusClicked(bool c);
	void catTypesMinusClicked(bool c);
	void catTypesDirChanged(int);
	void catTypesExeChanged(int);
	void catDepthChanged(int);
	void catProgressUpdated(float);
	void catRescanClicked(bool);
};


#endif