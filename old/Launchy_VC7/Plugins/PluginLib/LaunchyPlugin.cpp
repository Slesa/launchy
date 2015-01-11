#include <stdafx.h>
#include <tchar.h> 
#include <vector>
#include <map>
#include <string>
#include <sstream>

using namespace std;

#include "LaunchyPlugin.h"

map<wstring,wstring> Storage;

void FreeSearchResult (SearchResult* sr) {
	free(sr->DisplayString);
	free(sr->FullPath);
	free(sr->Location);
	DestroyIcon(sr->DisplayIcon);
}

/// <summary>
/// Blah Blah blah
/// </summary>
/// <param name="str">Use this str</param>
TCHAR* string2TCHAR(wstring str) {
	TCHAR* dest = (TCHAR*) malloc(sizeof(TCHAR) * (str.length() + 1));
	if (dest == NULL) exit(1);
	_tcscpy(dest, str.c_str());
	return dest;
}

unsigned long GenerateID(wstring str) {
    unsigned long hash = 5381;
	for(uint i = 0; i < str.size(); i++) {
		hash = ((hash << 5) + hash) + str[i];
	}
    return hash;
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
	if (FullPath == L"")
		sr.FullPath = string2TCHAR(DisplayString);
	else
		sr.FullPath = string2TCHAR(FullPath);
	sr.Location = string2TCHAR(Location);
	sr.DisplayIcon = icon;
	return sr;
}


TCHAR* StringVectorToTCHAR( vector<wstring>& Strings) {
	int size = 0;
	for(uint i = 0; i < Strings.size(); i++) {
		size += (int) (Strings[i].length() + 1);
	}
	TCHAR* out = (TCHAR*) malloc(sizeof(TCHAR) * size);
	if (out == NULL) exit(1);
	TCHAR* cur = out;
	for(uint i = 0; i < Strings.size(); i++) {
		_tcscpy(cur, Strings[i].c_str());
		cur += Strings[i].length()+1;
	}
	return out;
}

SearchResult* ResultVectorToArray(vector<SearchResult> results) {
	SearchResult* res = (SearchResult*) malloc(sizeof(SearchResult) * results.size());
	if(res == NULL) exit(1);

	SearchResult* cur = res;
	for(uint i = 0; i < results.size(); i++) {
		*cur = results[i];
		cur += 1;
	}
	return res;
}

TCHAR* SerializeStringMap(map<wstring,wstring> m) {
	int size = 0;
	for(map<wstring,wstring>::iterator it = m.begin(); it != m.end(); ++it) {
		size += (int) it->first.length() + 1;
		size += (int) it->second.length() + 1;
	}
	TCHAR* out = (TCHAR*) malloc(sizeof(TCHAR)* size);
	TCHAR* cur = out;
	for(map<wstring,wstring>::iterator it = m.begin(); it != m.end(); ++it) {
		_tcscpy(cur, it->first.c_str());
		cur += it->first.length() + 1;
		_tcscpy(cur, it->second.c_str());
		cur += it->second.length() + 1;
	}
	return out;
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


// Storage interface with Launchy executable
void PluginSetStorage(int NumItems, TCHAR* ItemNames, TCHAR* ItemValues) {
	Storage.clear();
	vector<wstring> names = TCHARListToVector(NumItems, ItemNames);
	vector<wstring> values = TCHARListToVector(NumItems, ItemValues);

	for(int i = 0; i < NumItems; i++) {
		Storage[names[i]] = values[i];
	}
}

// Storage interface with Launchy executable
void PluginGetStorage(int* NumItems, TCHAR** ItemNames, TCHAR** ItemValues) {
	// Have the plugin store its options
	PluginSaveOptions();

	map<wstring,wstring>::iterator it;
	vector<wstring> names;
	vector<wstring> values;
	for(it = Storage.begin(); it != Storage.end(); ++it) {
		names.push_back(it->first);
		values.push_back(it->second);
	}

	*NumItems = names.size();
	*ItemNames = StringVectorToTCHAR(names);
	*ItemValues = StringVectorToTCHAR(values);
}

void StoreString(wstring name, wstring value) {
	Storage[name] = value;
}

wstring RetrieveString(wstring name) {
	if (Storage.count(name) == 0) {
		return L"";
	}

	return Storage[name];
}

void StoreInt(wstring name, int value) {
	wostringstream ss;
	ss << value;
	Storage[name] = ss.str();
}

int RetrieveInt(wstring name, bool& exists) {
	exists = true;
	if (Storage.count(name) == 0) {
		exists = false;
		return -1;
	}

	wistringstream ss(Storage[name]);
	int out;
	ss >> out;
	return out;
}

void StoreFloat(wstring name, float value) {
	wostringstream ss;
	ss << value;
	Storage[name] = ss.str();
}

float RetrieveFloat(wstring name, bool& exists) {
	exists = true;
	if (Storage.count(name) == 0) {
		exists = false;
		return 0.0;
	}
	wistringstream ss(Storage[name]);
	float out;
	ss >> out;
	return out;
}
