// Controly.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "Controly.h"
#include "LaunchyPlugin.h"
#include <shellapi.h>
#include <string>
#include <vector>
#include <assert.h>
#include <cpl.h>

using namespace std;
HINSTANCE hInstance;

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
/*
	WNDCLASSEX WndClsEx;

	// Create the application window
	WndClsEx.cbSize        = sizeof(WNDCLASSEX);
	WndClsEx.style         = CS_HREDRAW | CS_VREDRAW;
	WndClsEx.lpfnWndProc   = WndProcedure;
	WndClsEx.cbClsExtra    = 0;
	WndClsEx.cbWndExtra    = 0;
	WndClsEx.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
	WndClsEx.hCursor       = LoadCursor(NULL, IDC_ARROW);
	WndClsEx.hbrBackground = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
	WndClsEx.lpszMenuName  = NULL;
	WndClsEx.lpszClassName = L"Controlyblah";
	WndClsEx.hInstance     = hInstance;
	WndClsEx.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

	// Register the application
	RegisterClassEx(&WndClsEx);

	HWND hwnd = CreateWindow(L"Controlyblah",
	     L"",
	     WS_BORDER,
	     CW_USEDEFAULT,
             CW_USEDEFAULT,
	     CW_USEDEFAULT,
	     CW_USEDEFAULT,
             NULL,
             NULL,
	     hInstance,
	     NULL);

//	ShowWindow(hwnd,SW_HIDE);
*/
	*iNumResults = 0;

	HWND hwnd = NULL;




	// Get the control panel applications
	TCHAR  infoBuf[32767];
	if (!GetSystemDirectory(infoBuf, 32767)) {
		return NULL;
	}

	vector<wstring> filenames;
	WIN32_FIND_DATA fdata;
	HANDLE dhandle;

	wstring path = infoBuf;
	path += L"\\*.cpl";


	if((dhandle = FindFirstFile(path.c_str(), &fdata)) == INVALID_HANDLE_VALUE) {
		return NULL;
	}

	filenames.push_back(fdata.cFileName);
	

	while(1) {
		if(FindNextFile(dhandle, &fdata)) {
			wstring tmp = fdata.cFileName;
			filenames.push_back(tmp);
		} else {
			if(GetLastError() == ERROR_NO_MORE_FILES) {
				break;
			} else {
				FindClose(dhandle);
				return NULL;
			}
		}
	}
	if(FindClose(dhandle) == 0) {
		return NULL;
	}

	
	vector<SearchResult> results;

	// Okay we have all of the control panel applications, load up the names
	for(int i = 0; i < filenames.size(); i++) {
		HINSTANCE LoadMe;
		path = infoBuf;
		path += L"\\";
		path +=  filenames[i];
		LoadMe = LoadLibrary(path.c_str());
		 
		// Check to see if the library was loaded successfully 
		if (LoadMe == 0) {
			continue;
		}
		 APPLET_PROC cpla = (APPLET_PROC)GetProcAddress(LoadMe, "CPlApplet");
		if (cpla == NULL) {
			FreeLibrary(LoadMe);
			continue;
		}
		LONG nInitResult = cpla(hwnd, CPL_INIT, 0, 0); 

		LONG NumberOfApplets = cpla(hwnd, CPL_GETCOUNT, 0, 0);
		for(int j = 0; j < NumberOfApplets; j++) {
			CPLINFO info;
			cpla(hwnd, CPL_INQUIRE, j, (LPARAM) &info);
			TCHAR CPName[32];
			LoadString(LoadMe, info.idName, CPName, 32);
			results.push_back(makeResult(CPName, path, L"", NULL));
			
		}
		
		cpla(hwnd, CPL_EXIT, 0, 0);
		FreeLibrary(LoadMe);

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


void PluginDoAction (int NumStrings, const TCHAR* Strings, const TCHAR* FinalString, const TCHAR* FullPath) {
	SHELLEXECUTEINFO ShExecInfo;
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = NULL;
	ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb = NULL;
	ShExecInfo.lpFile = FullPath;
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
	wstring tmp = L"Controly";
	return string2TCHAR(tmp);
}

TCHAR* PluginGetDescription() {
	wstring tmp = L"Launches Control Panel Applications";
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
