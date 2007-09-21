#ifndef ICON_DELEGATE
#define ICON_DELEGATE

#include <QAbstractItemDelegate>
#define ROLE_FULL Qt::UserRole
#define ROLE_SHORT Qt::UserRole + 1
#define ROLE_ICON Qt::UserRole + 2

class IconDelegate : public QAbstractItemDelegate
{
	Q_OBJECT

private:
	QColor color;
	QString family;
	int size;
	int weight;
	int italics;

public:

	IconDelegate(QObject *parent = 0);

	void paint(QPainter *painter, const QStyleOptionViewItem &option,
		const QModelIndex &index) const;

	QSize sizeHint(const QStyleOptionViewItem &option,
		const QModelIndex &index ) const;

	void setColor(QString line) {
		if (!line.contains(","))
			color = QColor(line);

		QStringList spl = line.split(",");
		if (spl.count() != 3) return;

		color = QColor(spl.at(0).toInt(), spl.at(1).toInt(), spl.at(2).toInt());
	}

	void setFamily(QString fam) { family = fam; }
	void setSize(int s) { size = s; }
	void setWeight(int w) { weight = w; }
	void setItalics(int i) { italics = i; }
};
#endif