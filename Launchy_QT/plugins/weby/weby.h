#ifndef WEBY_H
#define WEBY_H

#ifdef WIN32
#define VC_EXTRALEAN
#define WINVER 0x05100
#define _WIN32_WINNT 0x0510	
#define _WIN32_WINDOWS 0x0510 
#define _WIN32_IE 0x0600

#include <windows.h>
#include <shlobj.h>
#endif

#ifndef _UNICODE
#define _UNICODE
#endif

#include "plugin_interface.h"

void runProgram(QString file, QString args);

class WebyPlugin : public QObject, public PluginInterface
{
	Q_OBJECT
	Q_INTERFACES(PluginInterface)

	uint HASH_WEBSITE;
	uint HASH_WEBY;
public:
	WebyPlugin() {
		HASH_WEBSITE = qHash(QString("website"));
		HASH_WEBY = qHash(QString("weby"));
	}
	~WebyPlugin() {}
	bool msg(int msgId, void* wParam = NULL, void* lParam = NULL); 

	void getLabels(QList<InputData>*);
	void getID(uint*);
	void getResults(QList<InputData>* id, QList<CatItem>* results);
	void getCatalog(QList<CatItem>* items);
	void launchItem(QList<InputData>*, CatItem*);
};



#endif