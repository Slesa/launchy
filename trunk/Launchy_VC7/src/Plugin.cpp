#include "StdAfx.h"
#include ".\plugin.h"
#include "DiskObject.h"
#include "Launchy.h"
#include "LaunchyDlg.h"
#include "LaunchySmarts.h"
#include "LaunchyDlg.h"


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

int GenerateNameTag(CString str) {
    int hash = 5381;
	for(int i = 0; i < str.GetLength(); i++) {
		hash = ((hash << 5) + hash) + str[i];
	}
    return hash;
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
		cur += Strings[i].GetLength() + 1;
	}
	return out;
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

Plugin::Plugin(void)
{
	LoadDlls();
	LoadRegExs();
}

Plugin::~Plugin(void)
{
	shared_ptr<Options> ops = ((CLaunchyDlg*)AfxGetMainWnd())->options;
	for(uint i = 0; i < loadedPlugins.size(); i++) {
		if (pfuncs[i].PluginGetStorage != NULL)
			GetStorage(i, ops.get());
		if (pfuncs[i].PluginClose != NULL)
			pfuncs[i].PluginClose();
		FreeLibrary(loadedPlugins[i].handle);
	}
	loadedPlugins.clear();
	pfuncs.clear();

}

void Plugin::LoadRegExs() 
{
	for(uint i = 0; i < loadedPlugins.size(); i++) {
		if (pfuncs[i].PluginGetRegexs == NULL) continue;

		int numStrings = 0;
		TCHAR* strings = pfuncs[i].PluginGetRegexs(&numStrings);
		vector<wstring> vstrings = TCHARListToVector(numStrings, strings);
		for(uint j = 0; j < vstrings.size(); j++) {
			wstring x = vstrings[j];
			boost::wregex tmp(vstrings[j],boost::regex::perl|boost::regex::icase);
			loadedPlugins[i].regexs.push_back(tmp);
		}
		pfuncs[i].PluginFreeStrings(strings);
	}
}

/*
	Determine if one of the plugins owns the searchTxt by 
	a regular expression match
*/
int Plugin::IsSearchOwned(CString search) 
{
	wstring searchS = search.GetBuffer();
	for(uint i = 0; i < loadedPlugins.size(); i++) {
		for(uint j = 0; j < loadedPlugins[i].regexs.size(); j++) {
			boost::wsmatch what;
			if(boost::regex_match(searchS, what, loadedPlugins[i].regexs[j])) {
				return i;
			}
		}
	}
	return -1;
}


unsigned long Plugin::GetPluginNameTag(int id) {
	return loadedPlugins[id].nametag;
}

HICON Plugin::GetIcon(int id) {
	HICON ret;
	ExtractIconEx(loadedPlugins[id].filename, 0, &ret, NULL, 1);

	return ret;
}

void Plugin::SendStorage(CString PluginName, PLUGINSETSTORAGE PluginSetStorage) {
	shared_ptr<Options> ops = ((CLaunchyDlg*)AfxGetMainWnd())->options;
	// Send the plugin its storage
	CStringArray valuenames, values;
	int numValues = ops->ini->GetNumValues(PluginName);
	for(int i = 0; i < numValues; i++) {
		CString valuename, value;
		ops->ini->GetIthKeyValue(PluginName, i, valuename, value);
		valuenames.Add(valuename);
		values.Add(value);
	}
	TCHAR* szValueNames = StringArrayToTCHAR(valuenames);
	TCHAR* szValues = StringArrayToTCHAR(values);
	PluginSetStorage(numValues, szValueNames, szValues);
	free(szValueNames);
	free(szValues);


}

void Plugin::LoadDlls(bool FirstLoad /* = true */) {
	shared_ptr<Options> ops = ((CLaunchyDlg*)AfxGetMainWnd())->options;

	CDiskObject disk;
	CStringArray files;
	disk.EnumFilesInDirectoryWithFilter(_T("*.dll"), _T("Plugins/"), files);
	for(int i = 0; i < files.GetCount(); i++) {
		CString path = _T("Plugins\\") + files[i];
		HINSTANCE LoadMe;
		LoadMe = LoadLibrary(path);
		 
		// Check to see if the library was loaded successfully 
		if (LoadMe == 0) {
			CString out;
			out.Format(_T("Plugin (%s) failed to load!"), files[i]);
			AfxMessageBox(out);
			continue;
		}
		struct DLLInstance di;
		di.handle = LoadMe;
		di.filename = path;
		di.description = L"";
		di.name = L"";
		
		PluginFunctions funcs;

		funcs.PluginGetRegexs = (PLUGINGETREGEXS)GetProcAddress(LoadMe,"PluginGetRegexs");
		funcs.PluginGetIndexItems = (PLUGINGETINDEXITEMS)GetProcAddress(LoadMe,"PluginGetIndexItems");
		funcs.PluginUpdateSearch = (PLUGINUPDATESEARCH)GetProcAddress(LoadMe,"PluginUpdateSearch");
		funcs.PluginFileOptions = (PLUGINFILEOPTIONS)GetProcAddress(LoadMe,"PluginFileOptions");
		funcs.PluginDoAction = (PLUGINDOACTION)GetProcAddress(LoadMe,"PluginDoAction");
		funcs.PluginGetIdentifiers = (PLUGINGETIDENTIFIERS)GetProcAddress(LoadMe,"PluginGetIdentifiers");
		funcs.PluginFreeResults = (PLUGINFREERESULTS)GetProcAddress(LoadMe,"PluginFreeResults");
		funcs.PluginFreeStrings = (PLUGINFREESTRINGS)GetProcAddress(LoadMe,"PluginFreeStrings");
		funcs.PluginGetSeparator = (PLUGINGETSEPARATOR)GetProcAddress(LoadMe, "PluginGetSeparator");
		funcs.PluginGetName = (PLUGINGETNAME)GetProcAddress(LoadMe, "PluginGetName");
		funcs.PluginGetDescription = (PLUGINGETDESCRIPTION)GetProcAddress(LoadMe, "PluginGetDescription");
		funcs.PluginCallOptionsDlg = (PLUGINCALLOPTIONSDLG)GetProcAddress(LoadMe, "PluginCallOptionsDlg");
		funcs.PluginClose = (PLUGINCLOSE)GetProcAddress(LoadMe, "PluginClose");
		funcs.PluginGetStorage = (PLUGINGETSTORAGE)GetProcAddress(LoadMe, "PluginGetStorage");
		funcs.PluginSetStorage = (PLUGINSETSTORAGE)GetProcAddress(LoadMe, "PluginSetStorage");
		funcs.PluginInitialize = (PLUGININITIALIZE)GetProcAddress(LoadMe, "PluginInitialize");
		funcs.PluginHasOptionsDlg = (PLUGINHASOPTIONSDLG)GetProcAddress(LoadMe, "PluginHasOptionsDlg");


		if (funcs.PluginGetName != NULL) {
			TCHAR* tmpName = funcs.PluginGetName();
			di.name = tmpName;
			funcs.PluginFreeStrings(tmpName);
			di.nametag = GenerateNameTag(di.name);
		}


		// Do we already know about this plugin? If so, continue
		bool seenThisInstance = false;
		for(int j = 0; j < allPlugins.size(); j++) {
			if (di.name == allPlugins[j].name) {
				seenThisInstance = true;	
			}
		}


		if (!seenThisInstance && funcs.PluginGetName != NULL && funcs.PluginSetStorage != NULL) {
			SendStorage(di.name, funcs.PluginSetStorage);
		}

		if (!seenThisInstance && funcs.PluginInitialize)
			funcs.PluginInitialize();

		if (funcs.PluginGetDescription != NULL) {
			TCHAR* tmpDescr = funcs.PluginGetDescription();
			di.description = tmpDescr;
			funcs.PluginFreeStrings(tmpDescr);
		}

		DLLProperties prop;

		prop.hasOptionsDlg = false;
		if (funcs.PluginHasOptionsDlg != NULL) 
			prop.hasOptionsDlg = funcs.PluginHasOptionsDlg();

		if (!seenThisInstance && FirstLoad && ops->LoadPlugin(di.name)) {
			loadedPlugins.push_back(di);
			pfuncs.push_back(funcs);
			prop.loaded = true;
		} else {
			FreeLibrary(LoadMe);
			prop.loaded = false;
		}

		if (!seenThisInstance) {
			prop.name = di.name;
			prop.filename = path;
			prop.description = di.description;
			allPlugins.push_back(prop);
		}		
	}
}




void Plugin::CallOptionsDlg(const DLLProperties & props, HWND wnd) {
	if (props.loaded) {
		// Find the dll in the loaded list and call its funcs
		for(int i = 0; i < loadedPlugins.size(); i++) {
			if (loadedPlugins[i].name == props.name) {
				if (pfuncs[i].PluginCallOptionsDlg == NULL)
					return;
				pfuncs[i].PluginCallOptionsDlg(wnd);
				break;
			}
		}
		return;
	}

	// Okay this DLL isn't loaded, need to load it up and call it
	HINSTANCE LoadMe;
	LoadMe = LoadLibrary(props.filename);
	if (LoadMe == 0) return;
	PLUGINCALLOPTIONSDLG PluginCallOptionsDlg = (PLUGINCALLOPTIONSDLG)GetProcAddress(LoadMe, "PluginCallOptionsDlg");
	PLUGINSETSTORAGE PluginSetStorage = (PLUGINSETSTORAGE)GetProcAddress(LoadMe, "PluginSetStorage");
	PLUGINGETNAME PluginGetName = (PLUGINGETNAME)GetProcAddress(LoadMe, "PluginGetName");
	PLUGINFREESTRINGS PluginFreeStrings = (PLUGINFREESTRINGS)GetProcAddress(LoadMe,"PluginFreeStrings");
	PLUGININITIALIZE PluginInitialize = (PLUGININITIALIZE)GetProcAddress(LoadMe,"PluginInitialize");
	PLUGINHASOPTIONSDLG PluginHasOptionsDlg = (PLUGINHASOPTIONSDLG)GetProcAddress(LoadMe, "PluginHasOptionsDlg");
	bool hasOptionsDlg = false;
	if (PluginHasOptionsDlg != NULL) 
		hasOptionsDlg = PluginHasOptionsDlg;

	if (hasOptionsDlg) {
		// Give the plugin its options
		if (PluginGetName != NULL && PluginSetStorage != NULL) {
			TCHAR* tmpName = PluginGetName();
			CString name = tmpName;
			PluginFreeStrings(tmpName);
			SendStorage(name, PluginSetStorage);
		}
		if (PluginInitialize)
			PluginInitialize();

		// Call up the options dialog
		if (PluginCallOptionsDlg != NULL)
			PluginCallOptionsDlg(wnd);
	}
	FreeLibrary(LoadMe);
}

void Plugin::GetStorage(int id, Options* ops) {
//	shared_ptr<Options> ops = ((CLaunchyDlg*)AfxGetMainWnd())->options;
//	if (ops == NULL) return;

	int numResults = 0;
	TCHAR* Names = NULL;
	TCHAR* Values = NULL;


	pfuncs[id].PluginGetStorage(&numResults, &Names, &Values);

	vector<wstring> vNames = TCHARListToVector(numResults, Names);
	vector<wstring> vValues = TCHARListToVector(numResults, Values);
	pfuncs[id].PluginFreeStrings(Names);
	pfuncs[id].PluginFreeStrings(Values);

	if (vNames.size() != vValues.size() || vNames.size() != numResults) 
		return;

	for(int i = 0; i < numResults; i++) {
		ops->ini->SetValue(loadedPlugins[id].name, vNames[i].c_str(), vValues[i].c_str());
	}
}

vector<FileRecordPtr> Plugin::GetIdentifiers(Options* ops) {
	vector<FileRecordPtr> PluginRecords;
	for(uint i = 0; i < loadedPlugins.size(); i++) {
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
			rec->usage = 0;
			//			rec->usage = 1000;
			rec->owner = (short) i;

			PluginRecords.push_back(rec);

			cur++;
		}
		pfuncs[i].PluginFreeResults(res, NumResults);
	

		// Now get the storage items
		if (pfuncs[i].PluginGetStorage != NULL) {
			GetStorage(i, ops);
		}
	}

	return PluginRecords;
}

shared_ptr<vector<FileRecordPtr> > Plugin::GetSearchOptions(int owner)
{
	shared_ptr<vector<FileRecordPtr> > out;
	out.reset(new vector<FileRecordPtr>);

/*	if (searchTxt == L"buff") {
		out.reset(new vector<FileRecordPtr>);
	}
*/
	if (owner == -1) {
		// This file isn't owned by anyone, we want to get a suite of options
		// from the available utilities such as "open, enqueue, send email" etc..

		for(uint i = 0; i < loadedPlugins.size(); i++) {
			if (pfuncs[i].PluginUpdateSearch == NULL) continue;
			int NumResults;
			TCHAR* szStrings = StringArrayToTCHAR(SearchStrings);
			SearchResult* res = pfuncs[i].PluginFileOptions( TabbedMatch->fullPath, (int) SearchStrings.GetCount(), szStrings, searchTxt, &NumResults);
			free(szStrings);
			SearchResult* cur = res;
			for(int j = 0; j < NumResults; j++) {
				FileRecordPtr rec(new FileRecord());
				rec->croppedName = cur->DisplayString;
				rec->fullPath = cur->FullPath;
				rec->isHistory = false;
				rec->lowName = rec->croppedName;
				rec->lowName.MakeLower();
				rec->usage = 0;
				rec->owner = (short) i;

				out->push_back(rec);
				cur++;
			}	
			pfuncs[i].PluginFreeResults(res, NumResults);
		}
	} else {

		if (pfuncs[owner].PluginUpdateSearch == NULL) return out;


		int NumResults;

		TCHAR* szStrings = StringArrayToTCHAR(SearchStrings);
		SearchResult* res = pfuncs[owner].PluginUpdateSearch((int) SearchStrings.GetCount(), szStrings, searchTxt, &NumResults);
		free(szStrings);

		SearchResult* cur = res;
		for(int j = 0; j < NumResults; j++) {
			FileRecordPtr rec(new FileRecord());
			rec->croppedName = cur->DisplayString;
			rec->fullPath = cur->FullPath;
			rec->isHistory = false;
			rec->lowName = rec->croppedName;
			rec->lowName.MakeLower();
			rec->usage = 0;
			rec->owner = (short) owner;

			out->push_back(rec);
			cur++;
		}	

		pfuncs[owner].PluginFreeResults(res, NumResults);
	}
	return out;
}



void Plugin::Launch(int PluginID, TCHAR* fullPath) 
{
	TCHAR* szStrings = StringArrayToTCHAR(SearchStrings);

	pfuncs[PluginID].PluginDoAction((int) SearchStrings.GetCount(), szStrings, searchTxt, fullPath);
	free(szStrings);
}

CString Plugin::GetSeparator(int PluginID)
{
	if (pfuncs[PluginID].PluginGetSeparator == NULL) return L" | ";
	TCHAR* sep = pfuncs[PluginID].PluginGetSeparator();
	CString tmp = sep;
	pfuncs[PluginID].PluginFreeStrings(sep);
	return tmp;
}


