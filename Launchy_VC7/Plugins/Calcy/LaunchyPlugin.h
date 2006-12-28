#ifndef LAUNCHY_PLUGIN_H
#define LAUNCHY_PLUGIN_H

// Create a Win32 dll
// Use Unicode Character Set

#include <string>
#include <vector>

using namespace std;


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

extern "C" __declspec(dllexport) void PluginFreeResults (SearchResult*, int num);
extern "C" __declspec(dllexport) void PluginFreeStrings (TCHAR* str);
#endif