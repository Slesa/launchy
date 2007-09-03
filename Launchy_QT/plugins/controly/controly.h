#ifndef controly_H
#define controly_H

#include "plugin_interface.h"



class controlyPlugin : public QObject, public PluginInterface
{
	Q_OBJECT
	Q_INTERFACES(PluginInterface)


public:
	uint HASH_controly;

public:
	controlyPlugin() {

		HASH_controly = qHash(QString("controly"));
	}
	~controlyPlugin() {}
	bool msg(int msgId, void* wParam = NULL, void* lParam = NULL); 

	void getID(uint*);
	void getName(QString*);
	void getCatalog(QList<CatItem>*);
	void init();
	QString getIcon();
	void getApps(QList<CatItem>* items);

};




#endif