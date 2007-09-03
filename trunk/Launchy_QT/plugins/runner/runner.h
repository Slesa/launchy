#ifndef runner_H
#define runner_H
#include "gui.h"
#include "globals.h"

#include "plugin_interface.h"



class runnerPlugin : public QObject, public PluginInterface
{
	Q_OBJECT
	Q_INTERFACES(PluginInterface)


public:
	uint HASH_WEBSITE;
	uint HASH_runner;
	QList<runnerCmd> cmds;

private:
	Gui* gui;
public:
	runnerPlugin() {
		gui = NULL;
		HASH_runner = qHash(QString("runner"));
	}
	~runnerPlugin() {}
	bool msg(int msgId, void* wParam = NULL, void* lParam = NULL); 

	void getID(uint*);
	void getName(QString*);
	void getCatalog(QList<CatItem>* items);
	void getResults(QList<InputData>* id, QList<CatItem>* results);
	void launchItem(QList<InputData>*, CatItem*);
	void doDialog(QWidget* parent);
	void endDialog(bool accept);
	void init();
	QString getIcon();

};

extern runnerPlugin* grunnerInstance;



#endif