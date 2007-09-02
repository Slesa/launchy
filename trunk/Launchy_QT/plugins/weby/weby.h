#ifndef WEBY_H
#define WEBY_H
#include "gui.h"
#include "globals.h"

#include "plugin_interface.h"



class WebyPlugin : public QObject, public PluginInterface
{
	Q_OBJECT
	Q_INTERFACES(PluginInterface)


public:
	uint HASH_WEBSITE;
	uint HASH_WEBY;
	QList<WebySite> sites;

private:
	Gui* gui;
public:
	WebyPlugin() {
		gui = NULL;
		HASH_WEBSITE = qHash(QString("website"));
		HASH_WEBY = qHash(QString("weby"));
	}
	~WebyPlugin() {}
	bool msg(int msgId, void* wParam = NULL, void* lParam = NULL); 

	void getLabels(QList<InputData>*);
	void getID(uint*);
	void getName(QString*);
	void getResults(QList<InputData>* id, QList<CatItem>* results);
	void getCatalog(QList<CatItem>* items);
	void launchItem(QList<InputData>*, CatItem*);
	void doDialog(QWidget* parent);
	void endDialog(bool accept);
	void init();
};

extern WebyPlugin* gWebyInstance;



#endif