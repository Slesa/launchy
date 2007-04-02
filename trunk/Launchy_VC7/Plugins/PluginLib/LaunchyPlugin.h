#ifndef LAUNCHY_PLUGIN_H
#define LAUNCHY_PLUGIN_H

// Create a Win32 dll
// Use Unicode Character Set

#include <string>
#include <vector>
#include <map>
#include "stdafx.h"

using namespace std;
typedef unsigned int uint;

extern map<wstring,wstring> Storage;

struct SearchResult {
	TCHAR* DisplayString; // Must be set
	TCHAR* FullPath; // Can be NULL
	TCHAR* Location; // Can be NULL
	HICON DisplayIcon; // Make sure you 0 this on creation!!
};


void FreeSearchResult (SearchResult* sr);

SearchResult makeResult(wstring DisplayString, wstring FullPath, wstring Location, HICON icon);
SearchResult* ResultVectorToArray(vector<SearchResult> results);
vector<wstring> TCHARListToVector(int numStrings, const TCHAR* Strings);
TCHAR* string2TCHAR(wstring str);
TCHAR* StringVectorToTCHAR( vector<wstring>& Strings);
unsigned long GenerateID(wstring str);
TCHAR* SerializeStringMap(map<wstring,wstring> m);


void StoreString(wstring name, wstring value);
wstring RetrieveString(wstring name);
void StoreInt(wstring name, int value);
int RetrieveInt(wstring name, bool& exists);
void StoreFloat(wstring name, float value);
float RetrieveFloat(wstring name, bool& exists);


extern "C" __declspec(dllexport) SearchResult* PluginGetIdentifiers (int* iNumResults);
extern "C" __declspec(dllexport) SearchResult* PluginUpdateSearch (int NumStrings, const TCHAR* Strings, const TCHAR* FinalString, int* NumResults);
extern "C" __declspec(dllexport) SearchResult* PluginFileOptions (const TCHAR* FullPath, int NumStrings, const TCHAR* Strings, const TCHAR* FinalString, int* NumResults);
extern "C" __declspec(dllexport) void PluginDoAction (int NumStrings, const TCHAR* Strings, const TCHAR* FinalString, const TCHAR* FullPath);
extern "C" __declspec(dllexport) TCHAR* PluginGetRegexs(int* iNumResults);
extern "C" __declspec(dllexport) TCHAR* PluginGetSeparator();
extern "C" __declspec(dllexport) void PluginFreeResults (SearchResult*, int num);
extern "C" __declspec(dllexport) void PluginFreeStrings (TCHAR* str);
extern "C" __declspec(dllexport) TCHAR* PluginGetName();
extern "C" __declspec(dllexport) TCHAR* PluginGetDescription();
extern "C" __declspec(dllexport) void PluginInitialize();
extern "C" __declspec(dllexport) void PluginClose();
extern "C" __declspec(dllexport) void PluginCallOptionsDlg();
extern "C" __declspec(dllexport) void PluginGetStorage(int* NumItems, TCHAR** ItemNames, TCHAR** ItemValues);
extern "C" __declspec(dllexport) void PluginSetStorage(int NumItems, TCHAR* ItemNames, TCHAR* ItemValues);
extern "C" __declspec(dllexport) bool PluginHasOptionsDlg(void);
void PluginSaveOptions();
#endif