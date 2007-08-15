#ifndef TYPES_H
#define TYPES_H

#include <QHash>
#include <QString>


class TypeRegistrar
{
	QHash<int, QString> types;
	QHash<QString, int> names;
public:
	TypeRegistrar();

	int registerName(QString name);

	int getType(QString name);
};

#endif