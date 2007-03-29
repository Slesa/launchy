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
	pfuncs.clear();

	for(uint i = 0; i < loadedPlugins.size(); i++) {
		FreeLibrary(loadedPlugins[i].handle);
	}
	loadedPlugins.clear();
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

void Plugin::LoadDlls() {
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


		if (funcs.PluginGetName != NULL) {
			TCHAR* tmpName = funcs.PluginGetName();
			di.name = tmpName;
			funcs.PluginFreeStrings(tmpName);
			di.nametag = GenerateNameTag(di.name);
		}

		if (funcs.PluginGetDescription != NULL) {
			TCHAR* tmpDescr = funcs.PluginGetDescription();
			di.description = tmpDescr;
			funcs.PluginFreeStrings(tmpDescr);
		}

		DLLProperties prop;

		if (funcs.PluginCallOptionsDlg != NULL) 
			prop.hasOptionsDlg = true;
		else
			prop.hasOptionsDlg = false;

		if (ops->LoadPlugin(di.name)) {
			loadedPlugins.push_back(di);		
			pfuncs.push_back(funcs);
			prop.loaded = true;
		} else {
			FreeLibrary(LoadMe);
			prop.loaded = false;
		}
		prop.name = di.name;
		prop.filename = path;
		prop.description = di.description;
		allPlugins.push_back(prop);
		
	}
}




void Plugin::CallOptionsDlg(const DLLProperties & props) {
	if (props.loaded) {
		// Find the dll in the loaded list and call its funcs
		for(int i = 0; i < loadedPlugins.size(); i++) {
			if (loadedPlugins[i].name == props.name) {
				if (pfuncs[i].PluginCallOptionsDlg == NULL)
					return;
				pfuncs[i].PluginCallOptionsDlg();
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
	if (PluginCallOptionsDlg != NULL)
		PluginCallOptionsDlg();
	FreeLibrary(LoadMe);
}

vector<FileRecordPtr> Plugin::GetIdentifiers() {
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