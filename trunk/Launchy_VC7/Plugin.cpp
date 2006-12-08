#include "StdAfx.h"
#include ".\plugin.h"
#include "DiskObject.h"

void FreeSearchResult (SearchResult* sr) {
	free(sr->DisplayString);
	free(sr->FullPath);
	free(sr->Location);
	DestroyIcon(sr->DisplayIcon);
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

			PluginRecords.push_back(rec);

			cur++;
		}
		pfuncs[i].PluginFreeResults(res, NumResults);
	}

	return PluginRecords;
}


