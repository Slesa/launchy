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

map<wstring,wstring> Commands;

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

	for(map<wstring,wstring>::iterator it = Commands.begin(); it != Commands.end(); ++it) {
		results.push_back(makeResult(it->first, it->first, L"Runner Command", NULL));
	}
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

void PluginDoAction (int NumStrings, const TCHAR* Strings, const TCHAR* FinalString, const TCHAR* FullPath) {
	vector<wstring> VStrings = TCHARListToVector(NumStrings, Strings);
	VStrings.push_back(FinalString);
//	wstring lowFinal(FinalString);
//	transform(lowFinal.begin(), lowFinal.end(), lowFinal.begin(), tolower);

	if (VStrings.size() <= 1) {
		return;
	}

	if (Commands.count(VStrings[0]) == 0) return;

	wstring format = Commands[VStrings[0]];
	if (format == L"") return;


	vector<wstring> split;
	Split(format, split, L'$');

	wstring finalString = split[0];

	for(int i = 1; i < split.size(); i++) {
		if (split[i][0] - L'0' == i && VStrings.size() >= i+1) {
			finalString += VStrings[i];
		}
		finalString += split[i].substr(1);
	}

	SHELLEXECUTEINFO ShExecInfo;
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = NULL;
	ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb = NULL;
	ShExecInfo.lpFile = finalString.c_str();
	ShExecInfo.lpParameters = NULL;
	ShExecInfo.lpDirectory = NULL;
	ShExecInfo.nShow = SW_NORMAL;
	ShExecInfo.hInstApp = NULL;

	BOOL ret = ShellExecuteEx(&ShExecInfo);	
	return;
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
		Commands[names[i]] = RetrieveString(names[i]);
	}
}

void PluginSaveOptions() {
	wstring names = L"";

	for(map<wstring,wstring>::iterator it = Commands.begin(); it != Commands.end(); ++it) {
		names += it->first;
		map<wstring,wstring>::iterator itmp = it;
		itmp++;
		if (itmp != Commands.end())
			names += L";";
		StoreString(it->first, it->second);
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

void PluginCallOptionsDlg(HWND parent) {
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_OPTIONS), parent, OptionsDlgProc);
}