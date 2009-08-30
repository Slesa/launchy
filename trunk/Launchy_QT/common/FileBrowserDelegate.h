#ifndef FILEBROWSERDELEGATE_H
#define FILEBROWSERDELEGATE_H

#include "FileBrowser.h"
#include <QItemDelegate>
#include <QModelIndex>
#include <QObject>
#include <QSize>

class FileBrowserDelegate : public QItemDelegate
{
	Q_OBJECT

public:
	FileBrowserDelegate(QObject *parent = 0, FileBrowser::BrowseType browseType = FileBrowser::File);

	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;

	void setEditorData(QWidget *editor, const QModelIndex &index) const;
	void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

	void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
	FileBrowser::BrowseType browseType;
};


#endif
