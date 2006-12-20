#include "StdAfx.h"
#include ".\plugin.h"
#include "DiskObject.h"
#include "Launchy.h"

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

TCHAR* StringArrayToTCHAR( CStringArray& Strings) {
	int size = 0;
	for(int i = 0; i < Strings.GetSize(); i++) {
		size += Strings[i].GetLength() + 1;
	}
	TCHAR* out = (TCHAR*) malloc(sizeof(TCHAR) * size);
	if (out == NULL) exit(1);
	TCHAR* cur = out;
	for(int i = 0; i < Strings.GetSize(); i++) {
		TCHAR* x = Strings[i].GetBuffer();
		_tcscpy(cur, Strings[i].GetBuffer());
		cur += Strings[i].GetLength();
	}
	return out;
}

Plugin::Plugin(void)
{
	LoadDlls();
}

Plugin::~Plugin(void)
{
	pfuncs.clear();

	for(int i = 0; i < loadedPlugins.size(); i++) {
		FreeLibrary(loadedPlugins[i]);
	}
}


void Plugin::LoadDlls() {
	CDiskObject disk;
	CStringArray files;
	disk.EnumFilesInDirectoryWithFilter(_T("*.dll"), _T("Plugins/"), files);
	for(int i = 0; i < files.GetCount(); i++) {
		CString path = _T("Plugins/") + files[i];
		HINSTANCE LoadMe;
		LoadMe = LoadLibrary(path);
		 
		// Check to see if the library was loaded successfully 
		if (LoadMe == 0) {
			CString out;
			out.Format(_T("Plugin (%s) failed to load!"), files[i]);
			AfxMessageBox(out);
			continue;
		}
		loadedPlugins.push_back(LoadMe);
		PluginFunctions funcs;

		funcs.PluginOwnsSearch = (PLUGINOWNSSEARCH)GetProcAddress(LoadMe,"?PluginOwnsSearch@@YGXHH@Z");
		funcs.PluginGetIndexItems = (PLUGINGETINDEXITEMS)GetProcAddress(LoadMe,"PluginGetIndexItems");
		funcs.PluginUpdateSearch = (PLUGINUPDATESEARCH)GetProcAddress(LoadMe,"PluginUpdateSearch");
		funcs.PluginDoAction = (PLUGINDOACTION)GetProcAddress(LoadMe,"PluginDoAction");
		funcs.PluginGetIdentifiers = (PLUGINGETIDENTIFIERS)GetProcAddress(LoadMe,"PluginGetIdentifiers");
		funcs.PluginFreeResults = (PLUGINFREERESULTS)GetProcAddress(LoadMe,"PluginFreeResults");
		pfuncs.push_back(funcs);
	}
}

vector<FileRecordPtr> Plugin::GetIdentifiers() {
	vector<FileRecordPtr> PluginRecords;
	for(int i = 0; i < loadedPlugins.size(); i++) {
		if (pfuncs[i].PluginGetIdentifiers == NULL)
			continue;
		int NumResults;
		SearchResult* res = pfuncs[i].PluginGetIdentifiers(&NumResults);
		SearchResult* cur = res;
		for(int j = 0; j < NumResults; j++) {
			FileRecordPtr rec(new FileRecord());
			rec->croppedName = cur->DisplayString;
			rec->fullPath = cur->FullPath;
			rec->isHistory = false;
			rec->lowName = rec->croppedName;
			rec->lowName.MakeLower();
			rec->usage = 1000;
			rec->owner = (short) i;

			PluginRecords.push_back(rec);

			cur++;
		}
		pfuncs[i].PluginFreeResults(res, NumResults);
	}

	return PluginRecords;
}


void Plugin::Launch(short PluginID) 
{
	TCHAR* szStrings = StringArrayToTCHAR(SearchStrings);
	pfuncs[PluginID].PluginDoAction(SearchStrings.GetCount(), szStrings);
	free(szStrings);
}