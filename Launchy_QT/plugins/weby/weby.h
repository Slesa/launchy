#ifndef WEBY_H
#define WEBY_H

#include "plugin_interface.h"

class WebyPlugin : public QObject, public PluginInterface
{
	Q_OBJECT
	Q_INTERFACES(PluginInterface)

	uint HASH_WEBSITE;
	uint HASH_WEBY;
public:
	WebyPlugin() {
		HASH_WEBSITE = qHash("website");
		HASH_WEBY = qHash("weby");
	}
	~WebyPlugin() {}
	bool msg(int msgId, void* wParam = NULL, void* lParam = NULL); 

	void getLabels(QList<InputData>*);
	void getName(QString*);
	void getResults(QList<InputData>* id, QList<CatItem>* results);
};



#endif