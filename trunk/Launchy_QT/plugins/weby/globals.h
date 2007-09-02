#ifndef GLOBALS_H
#define GLOBALS_H
#include <QString>

struct WebySite {
	QString base;
	QString name;
	QString query;
};

struct Bookmark {
	QString url;
	QString shortcut;
	QString name;
};

#endif