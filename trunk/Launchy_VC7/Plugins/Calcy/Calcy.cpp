// Calcy.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "Calcy.h"
#include "LaunchyPlugin.h"
#include <shellapi.h>
#include <string>
#include <vector>
#include <assert.h>
#include <math.h>
#include "Main.h"
#include <sstream>



using namespace std;

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
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
	*iNumResults = 0;
	return NULL;
}

TCHAR* PluginGetRegexs(int* iNumResults)
{
	*iNumResults = 1;
	wstring regex = L"[\\d\\s\\-\\+\\*\\/\\(\\)\\.]+";
	return string2TCHAR(regex);
}


SearchResult* PluginUpdateSearch (int NumStrings, const TCHAR* Strings, const TCHAR* FinalString, int* NumResults) {
	double result;

	if (DoCalculation(FinalString, result)) {
		vector<SearchResult> results;
		wostringstream out;
		out << result;
		results.push_back(makeResult(out.str(), FinalString, L"", NULL));

		*NumResults = 1;
		return ResultVectorToArray(results);
	}

	*NumResults = 0;
	return NULL;
}

SearchResult* PluginFileOptions (const TCHAR* FullPath, int NumStrings, const TCHAR* Strings,  const TCHAR* FinalString,  int* NumResults) {
	*NumResults = 0;
	return NULL;
}


void PluginDoAction (int NumStrings, const TCHAR* Strings, const TCHAR* FinalString, const TCHAR* FullPath) {
	return;
}

TCHAR* PluginGetSeparator() {
	wstring tmp = L" | ";
	return string2TCHAR(tmp);
}

TCHAR* PluginGetName() {
	wstring tmp = L"Calcy";
	return string2TCHAR(tmp);
}

TCHAR* PluginGetDescription() {
	wstring tmp = L"A simple calculator utility";
	return string2TCHAR(tmp);
}

void PluginClose() {
	return;
}


void PluginInitialize() {
	return;
}

void PluginSaveOptions() {
	return;
}

void PluginCallOptionsDlg() {
	return;
}

bool PluginHasOptionsDlg() {
	return false;
}