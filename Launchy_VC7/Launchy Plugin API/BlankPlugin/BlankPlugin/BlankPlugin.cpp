// BlankPlugin.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "LaunchyPlugin.h"

#ifdef _MANAGED
#pragma managed(push, off)
#endif

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
    return TRUE;
}

#ifdef _MANAGED
#pragma managed(pop)
#endif

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
	return;
}

TCHAR* PluginGetSeparator() {
	wstring tmp = L" | ";
	return string2TCHAR(tmp);
}

TCHAR* PluginGetName() {
	wstring tmp = L"BlankPlugin";
	return string2TCHAR(tmp);
}

TCHAR* PluginGetDescription() {
	wstring tmp = L"This Plugin Does Nothing";
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

void PluginCallOptionsDlg(HWND parent) {
	return;
}

bool PluginHasOptionsDlg() {
	return false;
}

