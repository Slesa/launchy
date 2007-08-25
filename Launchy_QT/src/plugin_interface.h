#ifndef PLUGIN_INTERFACE_H
#define PLUGIN_INTERFACE_H

#include <QObject>
#include "catalog.h"
/*
typedef TCHAR* (* PLUGINGETREGEXS) (int*);
typedef IndexItems (* PLUGINGETINDEXITEMS) (void);
typedef SearchResult* (* PLUGINUPDATESEARCH) (	int NumStrings, const TCHAR* Strings, const TCHAR* FinalString, int* NumResults);
typedef SearchResult* (* PLUGINFILEOPTIONS) (const TCHAR* FullPath, int NumStrings, const TCHAR* Strings,  const TCHAR* FinalString, int* NumResults );
typedef void (* PLUGINDOACTION) (	int NumStrings, const TCHAR* Strings, const TCHAR* FinalString, const TCHAR* FullPath);


typedef void (* PLUGINADDINDEXITEMS) (IndexItems);
typedef  SearchResult* (* PLUGINGETIDENTIFIERS) (int*);
typedef void (* PLUGINFREERESULTS) ( SearchResult*, int);
typedef void (* PLUGINFREESTRINGS) ( TCHAR* );
typedef TCHAR* (* PLUGINGETSEPARATOR) (void);
typedef TCHAR* (* PLUGINGETNAME) (void);
typedef TCHAR* (* PLUGINGETDESCRIPTION) (void);
typedef void (* PLUGINCALLOPTIONSDLG) (HWND);
typedef void (* PLUGINCLOSE) (void);
typedef void (* PLUGININITIALIZE) (void);
typedef void (* PLUGINGETSTORAGE) (int* NumItems, TCHAR** ItemNames, TCHAR** ItemValues);
typedef void (* PLUGINSETSTORAGE) (int NumItems, TCHAR* ItemNames, TCHAR* ItemValues);
typedef bool (* PLUGINHASOPTIONSDLG) (void);
*/
#define MSG_GET_NAME 0
#define MSG_GET_LABELS 1
#define MSG_GET_RESULTS 2

/*
#define MSG_APPLY_LABELS 0			// label an item
#define MSG_GET_CATALOG_ITEMS 1		// get global catalog items
#define MSG_PERFORM_SEARCH 2		// get search results for an object
#define MSG_LAUNCH 3				// run the object
#define MSG_GET_SEPARATOR 4			// get the seperator symbol, e.g. "|"
#define MSG_GET_DESCRIPTION 5		// get a qstring description of this plugin
#define MSG_CALL_OPTIONS_DLG 6		// call the options dialog
#define MSG_HAS_OPTIONS_DLG 7		// does the plugin have an options dialog?
#define MSG_GET_STORAGE 8			// launchy stores the plugin's QHash<QString,QVariant> to disk
#define MSG_GET_NAME 9
*/

class PluginInterface
{
public:
	virtual ~PluginInterface() {};
	virtual bool msg(int msgId, void* wParam = NULL, void* lParam = NULL) = 0;
};

 Q_DECLARE_INTERFACE(PluginInterface,
                     "net.launchy.PluginInterface/1.0")



#endif