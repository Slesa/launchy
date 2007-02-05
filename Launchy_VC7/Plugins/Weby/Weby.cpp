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
	results.push_back(makeResult(L"Google", L"Google.Weby", L"", NULL));
	results.push_back(makeResult(L"Live Search", L"Live Search.Weby", L"", NULL));
	results.push_back(makeResult(L"MSN",L"MSN.Weby", L"", NULL));
	results.push_back(makeResult(L"Yahoo", L"Yahoo.Weby", L"", NULL));
	results.push_back(makeResult(L"Weather", L"Weather.Weby", L"", NULL));
	results.push_back(makeResult(L"Amazon", L"Amazon.Weby", L"", NULL));
	results.push_back(makeResult(L"Wikipedia", L"Wikipedia.Weby", L"", NULL));
	results.push_back(makeResult(L"Dictionary", L"Dictionary.Weby", L"", NULL));
	results.push_back(makeResult(L"Thesaurus", L"Thesaurus.Weby", L"", NULL));
	results.push_back(makeResult(L"IMDB", L"IMDB.Weby", L"", NULL));
	results.push_back(makeResult(L"Netflix", L"Netflix.Weby", L"", NULL));
	results.push_back(makeResult(L"MSDN WebSearch", L"MSDN WebSearch.Weby", L"", NULL));
	results.push_back(makeResult(L"E-Mail",L"E-Mail.Weby",L"",NULL));

	*iNumResults = (int) results.size();
	return ResultVectorToArray(results);
}

TCHAR* PluginGetRegexs(int* iNumResults)
{
	vector<wstring> vect;
	vect.push_back(L"^www.*$");
	vect.push_back(L"^http.*$");
	*iNumResults = (int) vect.size();
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
		wstring newFS = FinalString;

		/*
		// Replace ' ' with %20
		wstring tmpString = FinalString;
		wstring newFS = L"";

		for(int i = 0; i < tmpString.size(); i++) {
			if (tmpString[i] == L' ')
				newFS += L"%20";
			else
				newFS += tmpString[i];
		}
*/
		if (VStrings[0] == L"Google") {
//			tmp = L"http://www.google.com/custom?q=";
//			tmp += newFS;
//			tmp += L"&client=pub-6689613646410036&forid=1&channel=4190529112&ie=ISO-8859-1&oe=ISO-8859-1&cof=GALT%3A%23008000%3BGL%3A1%3BDIV%3A%23336699%3BVLC%3A663399%3BAH%3Acenter%3BBGC%3AFFFFFF%3BLBGC%3A336699%3BALC%3A0000FF%3BLC%3A0000FF%3BT%3A000000%3BGFNT%3A0000FF%3BGIMP%3A0000FF%3BFORID%3A1&hl=en";
			tmp = L"http://www.google.com/search?q=";
			tmp += newFS;
		}
		else if (VStrings[0] == L"Live Search") {
			tmp += L"http://search.live.com/results.aspx?q=";
			tmp += newFS;
		}
		else if (VStrings[0] == L"Yahoo") {
			tmp += L"http://search.yahoo.com/search?p=";
			tmp += newFS;
		}
		else if (VStrings[0] == L"MSN") {
			tmp += L"http://search.msn.com/results.aspx?q=";
			tmp += newFS;
		}
		else if (VStrings[0] == L"Weather") {
			tmp = L"http://www.weather.com/weather/local/";
			tmp += newFS;
		}
		else if (VStrings[0] == L"Amazon") {
			tmp = L"http://www.amazon.com/gp/search/?keywords=";
			tmp += newFS;
		}
		else if (VStrings[0] == L"Wikipedia") {
			tmp = L"http://www.wikipedia.com/wiki/Special:Search?search=";
			tmp += newFS;
		}
		else if (VStrings[0] == L"Dictionary") {
			tmp = L"http://www.dictionary.com/browse/";
			tmp += newFS;
		}
		else if (VStrings[0] == L"Thesaurus") {
			tmp = L"http://www.thesaurus.com/browse/";
			tmp += newFS;
		}
		else if (VStrings[0] == L"IMDB") {
			tmp = L"http://www.imdb.com/find?q=";
			tmp += newFS;
		}
		else if (VStrings[0] == L"Netflix") {
			tmp = L"http://www.netflix.com/Search?v1=";
			tmp += newFS;
		}
		else if (VStrings[0] == L"MSDN WebSearch") {
			tmp = L"http://search.msdn.microsoft.com/search/default.aspx?siteId=0&tab=0&query=";
			tmp += newFS;
		}
		else if (VStrings[0] == L"E-Mail") {
			tmp = L"mailto:";
			tmp += newFS;
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

TCHAR* PluginGetName() {
	wstring tmp = L"Weby";
	return string2TCHAR(tmp);
}