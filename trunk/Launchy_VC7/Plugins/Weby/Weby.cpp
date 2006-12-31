// Weby.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "Weby.h"
#include "LaunchyPlugin.h"
#include <shellapi.h>
#include <string>
#include <vector>
#include <assert.h>

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
	vector<SearchResult> results;
	results.push_back(makeResult(L"Google", L"", L"", NULL));
	results.push_back(makeResult(L"Weather", L"", L"", NULL));
	results.push_back(makeResult(L"Amazon", L"", L"", NULL));
	results.push_back(makeResult(L"Wikipedia", L"", L"", NULL));
	results.push_back(makeResult(L"Dictionary", L"", L"", NULL));
	results.push_back(makeResult(L"Thesaurus", L"", L"", NULL));
	results.push_back(makeResult(L"IMDB", L"", L"", NULL));

	*iNumResults = 7;
	return ResultVectorToArray(results);
}

TCHAR* PluginGetRegexs(int* iNumResults)
{
	*iNumResults = 2;
	vector<wstring> vect;
	vect.push_back(L"^www.*$");
	vect.push_back(L"^http.*$");
	return StringVectorToTCHAR(vect);
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
	vector<wstring> VStrings = TCHARListToVector(NumStrings, Strings);

	wstring tmp = L"";

	if (NumStrings == 0) {
		tmp = FinalString;
	}

	else if (NumStrings == 1) {
		if (VStrings[0] == L"Google") {
			tmp = L"http://www.google.com/search?q=";
			tmp += FinalString;
		}
		else if (VStrings[0] == L"Weather") {
			tmp = L"http://www.weather.com/weather/local/";
			tmp += FinalString;
		}
		else if (VStrings[0] == L"Amazon") {
			tmp = L"http://www.amazon.com/gp/search/?keywords=";
			tmp += FinalString;
		}
		else if (VStrings[0] == L"Wikipedia") {
			tmp = L"http://www.wikipedia.com/wiki/Special:Search?search=";
			tmp += FinalString;
		}
		else if (VStrings[0] == L"Dictionary") {
			tmp = L"http://www.dictionary.com/browse/";
			tmp += FinalString;
		}
		else if (VStrings[0] == L"Thesaurus") {
			tmp = L"http://www.thesaurus.com/browse/";
			tmp += FinalString;
		}
		else if (VStrings[0] == L"IMDB") {
			tmp = L"http://www.imdb.com/find?q=";
			tmp += FinalString;
		}
		else {
			tmp = L"http://www.google.com/search?q=";
		}
	}


	SHELLEXECUTEINFO ShExecInfo;
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = NULL;
	ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb = NULL;
	ShExecInfo.lpFile = tmp.c_str();
	ShExecInfo.lpParameters = NULL;
	ShExecInfo.lpDirectory = NULL;
	ShExecInfo.nShow = SW_NORMAL;
	ShExecInfo.hInstApp = NULL;

	BOOL ret = ShellExecuteEx(&ShExecInfo);
	
}

TCHAR* PluginGetSeparator() {
	wstring tmp = L" | ";
	return string2TCHAR(tmp);
}