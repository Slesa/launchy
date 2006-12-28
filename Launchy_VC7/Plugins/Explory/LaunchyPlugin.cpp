#include "stdafx.h"

#include <tchar.h> 
#include <vector>

using namespace std;

#include "LaunchyPlugin.h"

void FreeSearchResult (SearchResult* sr) {
	free(sr->DisplayString);
	free(sr->FullPath);
	free(sr->Location);
	DestroyIcon(sr->DisplayIcon);
}

TCHAR* string2TCHAR(wstring str) {
	TCHAR* dest = (TCHAR*) malloc(sizeof(TCHAR) * (str.length() + 1));
	if (dest == NULL) exit(1);
	_tcscpy(dest, str.c_str());
	return dest;
}


vector<wstring> TCHARListToVector(int numStrings,  const TCHAR* Strings) {
	vector<wstring> out;
	const TCHAR* cur = Strings;
	for(int i = 0; i < numStrings; i++) {
		out.push_back(cur);
		cur += out[i].length() + 1;
	}
	return out;
}

SearchResult makeResult(wstring DisplayString, wstring FullPath, wstring Location, HICON icon) {
	SearchResult sr;

	sr.DisplayString = string2TCHAR(DisplayString);
	sr.FullPath = string2TCHAR(FullPath);
	sr.Location = string2TCHAR(Location);
	sr.DisplayIcon = icon;
	return sr;
}


TCHAR* StringVectorToTCHAR( vector<wstring>& Strings) {
	int size = 0;
	for(int i = 0; i < Strings.size(); i++) {
		size += Strings[i].length() + 1;
	}
	TCHAR* out = (TCHAR*) malloc(sizeof(TCHAR) * size);
	if (out == NULL) exit(1);
	TCHAR* cur = out;
	for(int i = 0; i < Strings.size(); i++) {
		_tcscpy(cur, Strings[i].c_str());
		cur += Strings[i].length()+1;
	}
	return out;
}

SearchResult* ResultVectorToArray(vector<SearchResult> results) {
	SearchResult* res = (SearchResult*) malloc(sizeof(SearchResult) * results.size());
	if(res == NULL) exit(1);

	SearchResult* cur = res;
	for(int i = 0; i < results.size(); i++) {
		*cur = results[i];
		cur += 1;
	}
	return res;
}

void PluginFreeStrings (TCHAR* str) {
	if (str != NULL)
		free(str);
}

void PluginFreeResults (SearchResult* sr, int num) {
	SearchResult* cur = sr;
	for(int i = 0; i < num; i++) {
		FreeSearchResult(cur);
		cur++;
	}
}