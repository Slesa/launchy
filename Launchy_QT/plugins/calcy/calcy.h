#ifndef CALCY_H
#define CALCY_H

#include "plugin_interface.h"



class calcyPlugin : public QObject, public PluginInterface
{
	Q_OBJECT
	Q_INTERFACES(PluginInterface)


public:
	uint HASH_CALCY;

public:
	calcyPlugin() {

		HASH_CALCY = qHash(QString("calcy"));
	}
	~calcyPlugin() {}
	bool msg(int msgId, void* wParam = NULL, void* lParam = NULL); 

	void getLabels(QList<InputData>*);
	void getID(uint*);
	void getName(QString*);
	void getResults(QList<InputData>* id, QList<CatItem>* results);
	void init();
	QString getIcon();

};




#endif