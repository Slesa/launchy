// Runny.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "Runny.h"
#include "LaunchyPlugin.h"
#include <shellapi.h>
#include <string>
#include <vector>
#include <assert.h>
#include <shlobj.h>
#include <map>
#include <boost/regex.hpp>
#include <stdio.h>
#include <tchar.h>
#include <stdlib.h>
#include "resource.h"
#include "RunnyDlg.h"

HINSTANCE hInstance;

vector<cmd> Commands;

using namespace std;
BOOL APIENTRY DllMain( HINSTANCE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{	
	hInstance = hModule;
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
    return TRUE;
}



bool PluginOwnsSearch (TCHAR* txt) 
{
	return false;
}




SearchResult* PluginGetIdentifiers (int* iNumResults)
{


	vector<SearchResult> results;


	*iNumResults = (int) results.size();
	return ResultVectorToArray(results);
}

TCHAR* PluginGetRegexs(int* iNumResults)
{
	*iNumResults = 0;

	return NULL;
}


SearchResult* PluginUpdateSearch (int NumStrings, const TCHAR* Strings, const TCHAR* FinalString, int* NumResults) {
	*NumResults = 0;
	return NULL;
}
SearchResult* PluginFileOptions (const TCHAR* FullPath, int NumStrings, const TCHAR* Strings, const TCHAR* FinalString, int* NumResults) 
 {
	*NumResults = 0;
	return NULL;
}


void PluginDoAction (int NumStrings, const TCHAR* Strings, const TCHAR* FinalString, const TCHAR* FullPath) {

	
}
void Split(wstring str, vector<wstring>& tokens, const TCHAR delim = ' ') {
	if (str.size() == 0) return;
	int start = 0;
	for(uint i = 0; i < str.size(); i++) {
		if (str[i] == delim) {
			if (i != 0) 
				tokens.push_back(str.substr(start,i-start));
			start = i+1;
		}
	}
	if (str[str.size()-1] != delim) {
		tokens.push_back(str.substr(start, str.size()-start));
	}
}


TCHAR* PluginGetSeparator() {
	wstring tmp = L" | ";
	return string2TCHAR(tmp);
}

TCHAR* PluginGetName() {
	wstring tmp = L"Runny";
	return string2TCHAR(tmp);
}

TCHAR* PluginGetDescription() {
	wstring tmp = L"Runs customized commands (with arguments)";
	return string2TCHAR(tmp);
}

void PluginInitialize() {
	vector<wstring> names;
	Split(RetrieveString(L"COMMAND_NAMES"), names, L';');
	for(int i = 0; i < names.size(); i++) {
		cmd c;
		c.name = names[i];
		c.format = RetrieveString(names[i]);
		Commands.push_back(c);
	}
}

void PluginSaveOptions() {
	wstring names = L"";
	for(int i = 0; i < Commands.size(); i++) {
		names += Commands[i].name;
		if (i != Commands.size() - 1)
			names += L";";
		StoreString(Commands[i].name, Commands[i].format);
	}
	StoreString(L"COMMAND_NAMES", names);
	return;
}

void PluginClose() {
	// PluginSaveOptions gets called just before this, nothing left to do..
	return;
}


bool PluginHasOptionsDlg() {
	return true;
}

void PluginCallOptionsDlg() {
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_OPTIONS), NULL, OptionsDlgProc);
}