#ifndef ICON_DELEGATE
#define ICON_DELEGATE


#include <QStyledItemDelegate>


#define ROLE_SHORT Qt::DisplayRole
#define ROLE_FULL Qt::ToolTipRole
#define ROLE_ICON Qt::DecorationRole


class IconDelegate : public QStyledItemDelegate
{
	Q_OBJECT

public:
	IconDelegate(QObject *parent = 0);

	void paint(QPainter *painter, const QStyleOptionViewItem &option,
		const QModelIndex &index) const;

	QSize sizeHint(const QStyleOptionViewItem &option,
		const QModelIndex &index ) const;

	void setColor(QString line, bool hi = false)
	{
		if (!line.contains(","))
			color = QColor(line);

		QStringList spl = line.split(",");
		if (spl.count() != 3) return;
		if (!hi)
			color = QColor(spl.at(0).toInt(), spl.at(1).toInt(), spl.at(2).toInt());
		else
			hicolor = QColor(spl.at(0).toInt(), spl.at(1).toInt(), spl.at(2).toInt());
	}

	void setFamily(QString fam) { family = fam; }
	void setSize(int s) { size = s; }
	void setWeight(int w) { weight = w; }
	void setItalics(int i) { italics = i; }

private:
	QColor color;
	QColor hicolor;
	QString family;
	int size;
	int weight;
	int italics;
};


#endif
