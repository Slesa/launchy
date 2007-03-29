/*
Launchy: Application Launcher
Copyright (C) 2005  Josh Karlin

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#include "StdAfx.h"
#include "LaunchySmarts.h"
#include <shlobj.h>
#include "LaunchyDlg.h"
#include "FileRecord.h"
#include "Launchy.h"
#include "launchysmarts.h"
#include "CArchiveEx.h"
#include ".\launchysmarts.h"
#include <boost/regex.hpp>
#include "Plugin.h"
//#define SECURITY_WIN32 1
//#include <Security.h>

// Code to get shell directories
#ifndef CSIDL_WINDOWS
#define CSIDL_WINDOWS                   0x0024      // from Platform SDK
#define CSIDL_SYSTEM                    0x0025      // GetSystemDirectory()
#define CSIDL_PROGRAM_FILES             0x0026      // C:\Program Files
typedef enum {
	SHGFP_TYPE_CURRENT  = 0,   // current value for user, verify it exists
	SHGFP_TYPE_DEFAULT  = 1,   // default value, may not exist
} SHGFP_TYPE;
#endif//CSIDL_WINDOWS




class ArchiveType : public CObject {
public:
	DECLARE_SERIAL( ArchiveType )
	ArchiveType() {}
	ArchiveType(CString str, int u, int n): name(str), usage(u), nametag(n) {}
	CString name;
	int usage;
	int nametag;
	void Serialize( CArchive& archive )
	{
		// call base class function first
		// base class is CObject in this case
		CObject::Serialize( archive );

		// now do the stuff for our specific class
		if( archive.IsStoring() )
			archive << name << usage << nametag;
		else
			archive >> name >> usage >> nametag;
	}
	ArchiveType( const ArchiveType &s ) {  // copy ctor 
		name = s.name;
		usage = s.usage;
		nametag = s.nametag;
	}
	ArchiveType& operator=( const ArchiveType &s )  {// assignment operator
		name = s.name;
		usage = s.usage;
		nametag = s.nametag;
		return *this;
	}
};


IMPLEMENT_SERIAL( ArchiveType, CObject, 1 )

template <> void AFXAPI SerializeElements <ArchiveType> ( CArchive& ar, 
														 ArchiveType* pArchs, INT_PTR nCount )
{
	for ( int i = 0; i < nCount; i++, pArchs++ )
	{
		// Serialize each ArchiveType object
		pArchs->Serialize( ar );
	}
}


bool less_than(const shared_ptr<FileRecord> a, const shared_ptr<FileRecord> b)
{

	if (a->isHistory) { return true; }
	if (b->isHistory) { return false; }

	bool localEqual = a->lowName == searchTxt;
	bool otherEqual = b->lowName == searchTxt;

	if (localEqual && !otherEqual)
		return true;
	if (!localEqual && otherEqual)
		return false;


	if(a->usage > b->usage)
		return true;
	if (a->usage < b->usage)
		return false;

	int localFind = a->lowName.Find(searchTxt);
	int otherFind = b->lowName.Find(searchTxt);

	if (localFind != -1 && otherFind == -1)
		return true;
	else if (localFind == -1 && otherFind != -1)
		return false;

	/*
	if (localFind != -1 && otherFind != -1) {
		if (localFind < otherFind)
			return true;
		else
			return false;
	}
*/
	int localLen = a->lowName.GetLength();
	int otherLen = b->lowName.GetLength();

	if (localLen < otherLen)
		return true;
	if (localLen > otherLen)
		return false;

	
	// Absolute tiebreaker to prevent loops
	if (a.get() < b.get()) 
		return true;
	else
		return false;

	return false;
}


LaunchySmarts::LaunchySmarts(void)
: lastUpdateTxt(_T(""))
{
	hMutex = CreateMutex( 
		NULL,                       // default security attributes
		FALSE,                      // initially not owned
		NULL);                      // unnamed mutex


	catFiles = 0;
	LoadFirstTime();
}

LaunchySmarts::~LaunchySmarts(void)
{
	getCatalogLock();
	shared_ptr<Options> ops =  ((CLaunchyDlg*)AfxGetMainWnd())->options;
	shared_ptr<Plugin> plugins = ((CLaunchyDlg*)AfxGetMainWnd())->plugins;
	ops->getLock();
	this->archiveCatalog(ops->get_dataPath(), plugins.get());
	ops->relLock();
	releaseCatalogLock();
}


int findCount(LaunchySmarts* smarts, CString s, CString path) {
	if (smarts->charMap.count(s[0]) == 0)
		return 0;
	for(uint i = 0; i < smarts->charMap[s[0]]->size(); i++) {
		if (path == smarts->charMap[s[0]]->at(i)->fullPath) {
			return smarts->charMap[s[0]]->at(i)->usage;
		}
	}
	return 0;
}



void ScanFiles(CArray<ArchiveType>& in, ScanBundle* bun, CArray<ArchiveType>& out)
{
	map<CString,bool> catalog;



	CString tmps;
	Options* ops = bun->ops;

	CMap<TCHAR, TCHAR&, bool, bool&> added;

	INT_PTR count = in.GetCount();


	map<int /*plugin id*/, map<CString /*FullPath*/, int /*Usage*/> > PluginCounts;

	// Scan for files
	for(int i = 0; i < count; i++) {


		if (in[i].nametag != -1) {
			PluginCounts[in[i].nametag][in[i].name] = in[i].usage;
			continue;
		}



		int lastDot = in[i].name.ReverseFind('.');
		if (lastDot == -1) continue;
		tmps = in[i].name.Mid(lastDot);
		tmps.MakeLower();

		FileRecordPtr rec(new FileRecord());
		rec->set(in[i].name, tmps, &bun->smarts->exeLauncher);

		if (in[i].usage != -1)
			rec->setUsage(in[i].usage);
		else
			rec->setUsage(findCount(bun->smarts, rec->lowName, rec->fullPath));


		ArchiveType at(in[i].name, rec->usage, -1);
		out.Add(at);
		//		if (catalog[rec->lowName] == true) continue;
		//		catalog[rec->lowName] = true;
		bun->catFiles += 1;
		added.RemoveAll();
		for(int i = 0; i < rec->lowName.GetLength( ); i++) {
			TCHAR c = rec->lowName[i];
			bun->charUsage[c] += 1;

			if (bun->charMap.count(c) == 0) {
				bun->charMap[c].reset(new vector<FileRecordPtr>());	
			}
			if (added[c] == false) {
				bun->charMap[c]->push_back(rec);
				added[c] = true;
			}
		}
	}
/*
	// Add the plugin names
	vector<FileRecordPtr> recs = bun->plugins->GetIdentifiers();

	added.RemoveAll();
	for(uint i = 0; i < recs.size(); i++) {
		
		FileRecordPtr rec = recs[i];
		int nametag = bun->plugins->GetPluginNameTag(rec->owner);

		// Loading from the original database?  Get the usage values..
		if (PluginCounts.count(nametag) != 0 && PluginCounts[nametag].count(rec->fullPath) != 0) {
			rec->setUsage(PluginCounts[nametag][rec->fullPath]);
		}

		// Refreshing the database?  Get the usage values from the last catalog..
		if (rec->usage == 0) {
			rec->setUsage(findCount(bun->smarts, rec->lowName, rec->fullPath));
		}


		ArchiveType at(rec->fullPath, rec->usage, nametag);
		out.Add(at);

		CMap<TCHAR, TCHAR&, bool, bool&> added;

		for(int i = 0; i < rec->lowName.GetLength( ); i++) {
			TCHAR c = rec->lowName[i];
			bun->charUsage[c] += 1;

			if (bun->charMap.count(c) == 0) {
				bun->charMap[c].reset(new vector<FileRecordPtr>());	
			}
			if (added[c] == false) {
				bun->charMap[c]->push_back(rec);
				added[c] = true;
			}
		}
	}
	*/
}


UINT ScanStartMenu(LPVOID pParam)
{
	ScanBundle* bun = (ScanBundle*) pParam;

	Options* ops = bun->ops;

	CStringArray files;
	CArray<ArchiveType> input;
	CArray<ArchiveType> smaller;
	CStringArray tmpFiles;
	files.SetSize(1024);
	tmpFiles.SetSize(1024);


	CDiskObject disk;

	vector<DirOptions> Directories = ops->get_Directories();

	map<CString, bool> globalTypeMap;
	vector<CString> GlobalTypes = bun->ops->get_Types();
	for(uint i = 0; i < GlobalTypes.size(); i++) {
		globalTypeMap[GlobalTypes[i]] = true;
	}

	for(uint i = 0; i < Directories.size(); i++) {
		map<CString, bool> localTypeMap;

		for(uint j = 0; j < Directories[i].types.size(); j++) {
			localTypeMap[Directories[i].types[j]] = true;
		}
		disk.EnumAllFiles(Directories[i].dir, tmpFiles);

		for(int j = 0; j < tmpFiles.GetCount(); j++) {
			int lastDot = tmpFiles[j].ReverseFind('.');
			if (lastDot == -1) continue;
			CString tmps = tmpFiles[j].Mid(lastDot);
			tmps.MakeLower();

			if (globalTypeMap[tmps] || localTypeMap[tmps]) {
				files.Add(tmpFiles[j]);
			}
		}
		tmpFiles.RemoveAll();

		if (globalTypeMap[_T(".directory")] || localTypeMap[_T(".directory")]) {
			CStringArray dirs;
			BOOL result = disk.EnumAllDirectories( Directories[i].dir, dirs);
			dirs.Add(Directories[i].dir);

			for(int i = 0; i < dirs.GetSize(); i++) {
				CString x = dirs[i].TrimRight(_T("\\")) + _T(".directory");
				files.Add(x);
			}
			dirs.RemoveAll();
		}
	}


	for(int i = 0; i < files.GetSize(); i++) {
		ArchiveType at(files[i], -1, -1);
		input.Add(at);
	}


	ScanFiles(input, bun, smaller);


	// Now replace the catalog files
	bun->smarts->getCatalogLock();

	bun->smarts->charMap = bun->charMap; // DIED ON THIS LINE?  IN A SEP. THREAD (I typed wiki then tab) Seems to have died in ~FileRecord
	bun->smarts->charUsage = bun->charUsage;
	bun->smarts->catFiles = bun->catFiles;

	bun->ops->getLock();
	bun->smarts->archiveCatalog(ops->get_dataPath(), bun->plugins);
	bun->ops->relLock();

	bun->smarts->releaseCatalogLock();
	::PostMessage(bun->dlg, LAUNCHY_DB_DONE, (WPARAM)0, (LPARAM)0);
	delete bun;


	return 0;
}

void LaunchySmarts::LoadCatalog(void)
{
	shared_ptr<Options> ops = ((CLaunchyDlg*)AfxGetMainWnd())->options;
	
	if (ops->get_Indexing()) {
		// Already indexing, abort
		return;
	}

	ops->set_Indexing(true);

	ScanBundle* bundle = new ScanBundle();
	bundle->smarts = this;
	bundle->ops = ((CLaunchyDlg*)AfxGetMainWnd())->options.get();
	bundle->plugins = ((CLaunchyDlg*)AfxGetMainWnd())->plugins.get();
	bundle->catFiles = 0;
	bundle->dlg = AfxGetMainWnd()->GetSafeHwnd();

	AfxBeginThread(ScanStartMenu, bundle, THREAD_PRIORITY_IDLE);
}



/*
When the program is launched, it's faster to just
read an old archive of the file names rather than
plow through the filesystem while the computer is
trying to start up.  This makes Launchy feel lighter.
*/

void LaunchySmarts::LoadFirstTime()
{
	CFile theFile;
	CString dir;
	dir.Format(_T("%s\\launchy.db"), ((CLaunchyDlg*)AfxGetMainWnd())->options->get_dataPath());


	if (!theFile.Open(dir, CFile::modeRead)) {
		LoadCatalog();
		return;
	}
	CArray<ArchiveType> archtypes;
	CArray<ArchiveType> smaller;
	CArchiveExt archive(&theFile, CArchive::load, 4096, NULL, _T(""), TRUE);

	try {
		archtypes.Serialize(archive);
		archive.Close();
		theFile.Close();
	} catch (...) {
		archive.Close();
		theFile.Close();
		LoadCatalog();
		return;
	}

	ScanBundle* bundle = new ScanBundle();
	bundle->smarts = this;
	bundle->ops = ((CLaunchyDlg*)AfxGetMainWnd())->options.get();
	bundle->plugins = ((CLaunchyDlg*)AfxGetMainWnd())->plugins.get();
	bundle->catFiles = 0;
	bundle->dlg = AfxGetMainWnd()->GetSafeHwnd();
	ScanFiles(archtypes, bundle, smaller);

	// Now replace the catalog files
	bundle->smarts->getCatalogLock();

	bundle->smarts->charMap = bundle->charMap;
	bundle->smarts->charUsage = bundle->charUsage;
	bundle->smarts->catFiles = bundle->catFiles;

	bundle->smarts->releaseCatalogLock();
	::PostMessage(bundle->dlg, LAUNCHY_DB_DONE, (WPARAM)0, (LPARAM)0);

	delete bundle;
}



void LaunchySmarts::Update(CString txt, bool UpdateDropdown, CString oneTimeHistory)
{
	CLaunchyDlg* pDlg = (CLaunchyDlg*) AfxGetMainWnd();
	if (pDlg == NULL)
		return;

	txt.MakeLower();
	txt.Replace(_T(" "), _T(""));


	lastUpdateTxt = txt;

	CString history = pDlg->options->GetAssociation(txt);
	// Very important that matches are cleared before
	// calling FindMatches() since findmatches() calls
	// findsearchowner which looks at matches[0]
	matches.clear();
	FindMatches(txt);

	// Set the preferred bit for the history match 	 
	size_t count = matches.size(); 	 
	for(size_t i = 0; i < count; i++) { 	 
		if ((history != L"" && matches[i]->fullPath == history) || (oneTimeHistory != L"" && matches[i]->fullPath == oneTimeHistory)) { 
			if (matches[i]->fullPath == oneTimeHistory || oneTimeHistory == _T("")) {
				matches[i]->isHistory = true; 	 
			}
		} 	 
	}
	sort(matches.begin(), matches.end(), less_than);

	// Unset the preferred bit for the history match 	 
	if (count > 0) 	 
		matches[0]->isHistory = false;


	int OwnerType;
	int owner = FindSearchOwner(OwnerType);

	if (matches.size() > 0) {

		HICON hNew;
		if (matches[0]->fullPath.Find(_T(".directory")) != -1) {
			hNew = pDlg->IconInfo.GetFolderIconHandle(false);
		} else {
			if (OwnerType != OWNER_LAUNCHY) {
				hNew = pDlg->plugins->GetIcon(owner);
			} else {
				hNew = pDlg->IconInfo.GetIconHandleNoOverlay(matches[0]->fullPath, false);
			}
		}
		HICON h = pDlg->IconPreview.SetIcon(hNew);
		if (h != hNew) {
			DestroyIcon(h);
		}
		pDlg->Preview.SetWindowText(matches[0]->croppedName);
	}
	else if (matches.size() == 0 && OwnerType != OWNER_LAUNCHY && OwnerType != OWNER_OPTIONS) {
		HICON hNew = pDlg->plugins->GetIcon(owner);
		HICON h = pDlg->IconPreview.SetIcon(hNew);
		if (h != hNew) DestroyIcon(h);
		pDlg->Preview.SetWindowText(_T(""));
	}	
	else {
		HICON h = pDlg->IconPreview.SetIcon(NULL);
		if (h != NULL) {
			DestroyIcon(h);
		}
		pDlg->Preview.SetWindowText(_T(""));
	}

	if (UpdateDropdown) {

		vector<DropItem*> oldData;
		for (int i = 0; i < pDlg->InputBox.GetCount(); i++) {
			oldData.push_back((DropItem*) pDlg->InputBox.GetItemDataPtr(i));
		}

		// Empty the combo box
		int count = pDlg->InputBox.GetCount();
		for(int i = count-1; i >= 0; i--) {
			pDlg->InputBox.SetItemDataPtr(i, NULL);
			pDlg->InputBox.DeleteString( i );
		}

		for(uint i = 0; i < oldData.size(); i++) {
			delete oldData[i];
		}

		int size = (int) matches.size();
		for(int i = 0; i < size && i < pDlg->options->listLength; i++) {
			CString full = matches[i]->fullPath;


			int ind = full.ReverseFind(_T('\\'));

			int index = pDlg->InputBox.AddString(matches[i]->croppedName);
			DropItem* data = new DropItem();


			data->longpath = full;
			if(matches[i]->owner == -1)
				data->lesspath = full.Left(ind+1).Right(pDlg->options->skin->listWidthInChars);
			else
				data->lesspath = matches[i]->fullPath.Right(pDlg->options->skin->listWidthInChars);;
			data->icon = NULL;
			data->owner = matches[i]->owner;
			pDlg->InputBox.SetItemDataPtr(index, (void*) data);
		}

	}

}

int LaunchySmarts::FindSearchOwner(int& OwnerType) {
	shared_ptr<Plugin> plugins = ((CLaunchyDlg*)AfxGetMainWnd())->plugins;


	if (matches.size() > 0) {
		if (matches[0]->owner != -1) {
			OwnerType = OWNER_MATCH;
			return matches[0]->owner;
		}
	}

	if (SearchStrings.GetCount() > 0 && SearchPluginID != -1) {
		OwnerType = OWNER_TABBED;
		return SearchPluginID;
	}

	if (SearchStrings.GetCount() > 0 && SearchPluginID == -1) {
		OwnerType = OWNER_OPTIONS;
		return -1;
	}

	int owner = plugins->IsSearchOwned(searchTxt);
	if (owner != -1) {
		OwnerType = OWNER_REGEX;
		return owner;
	}


	OwnerType = OWNER_LAUNCHY;
	return -1;
}

void LaunchySmarts::FindMatches(CString txt)
{
	shared_ptr<Plugin> plugins = ((CLaunchyDlg*)AfxGetMainWnd())->plugins;
	int OwnerType;
	int owner = FindSearchOwner(OwnerType);

	if (OwnerType != OWNER_LAUNCHY) {
		shared_ptr<vector<FileRecordPtr> > pluginMatches = plugins->GetSearchOptions(owner);
		for(size_t i = 0; i < pluginMatches->size(); i++) {
				matches.push_back(pluginMatches->at(i));
		}
	} else {

		// The search is not owned, search through the general catalog

		getCatalogLock();
		//	txt.MakeLower();

		bool set = false;
		TCHAR mostInfo = -1;


		// Find the character with the most amount of information
		for(int i = 0; i < txt.GetLength(); i++) {
			TCHAR c = txt[i];
			if (charUsage[c] < charUsage[mostInfo] || !set) {
				mostInfo = c;
				set = true;
			}
		}

		if (charMap.count(mostInfo) != 0) {
			size_t count = charMap[mostInfo]->size();
			for(size_t i = 0; i < count; i++) {
				if (Match(charMap[mostInfo]->at(i), txt)) {
					matches.push_back(charMap[mostInfo]->at(i));
				}
			}
		}

		releaseCatalogLock();
	}
}



inline BOOL LaunchySmarts::Match(FileRecordPtr record, CString txt)
{
	int size = record->lowName.GetLength();
	int txtSize = txt.GetLength();
	int curChar = 0;

	for(int i = 0; i < size; i++) {
		if (record->lowName[i] == txt[curChar]) {
			curChar++;
			if (curChar >= txtSize) {
				return true;
			}
		} 
	}
	return false;
}

void LaunchySmarts::Launch(void)
{
	shared_ptr<Plugin> plugins = ((CLaunchyDlg*)AfxGetMainWnd())->plugins;
	int RegExOwner = plugins->IsSearchOwned(searchTxt);

	if (SearchStrings.GetSize() > 0 && SearchPluginID != -1) {
		if (matches.size() > 0) {
			matches[0]->setUsage(matches[0]->usage + 1);
			plugins->Launch(SearchPluginID, matches[0]->fullPath.GetBuffer());
		}
		else {
			TabbedMatch->setUsage(TabbedMatch->usage + 1);
			plugins->Launch(SearchPluginID, L"");
		}
	}

	// Haven't selected anything after we tabbed on a file/folder, launch matches[0]
	else if (SearchStrings.GetSize() > 0 && SearchPluginID == -1) {
		if (matches.size() > 0) {
			matches[0]->setUsage(matches[0]->usage + 1);
			exeLauncher.Run(matches[0]);		
		}
		else {
			TabbedMatch->setUsage(TabbedMatch->usage + 1);
			shared_ptr<FileRecord> tmp = TabbedMatch;
			exeLauncher.Run(tmp);
		}
	}

	else if (RegExOwner != -1) {
		if (matches.size() > 0) {
			matches[0]->setUsage(matches[0]->usage + 1);
			plugins->Launch(RegExOwner, matches[0]->fullPath.GetBuffer());
		} else
			plugins->Launch(RegExOwner, L"");
	}

	else if(matches.size() > 0) {
		matches[0]->setUsage(matches[0]->usage + 1);
		exeLauncher.Run(matches[0]);
	}
}

BOOL LaunchySmarts::GetShellDir(int iType, CString& szPath)
{
	HINSTANCE hInst = ::LoadLibrary( _T("shell32.dll") );
	if ( NULL == hInst )
	{
		ASSERT( 0 );
		return FALSE;
	}

	HRESULT (__stdcall *pfnSHGetFolderPath)( HWND, int, HANDLE, DWORD, LPWSTR );


	pfnSHGetFolderPath = reinterpret_cast<HRESULT (__stdcall *)( HWND, int, HANDLE, DWORD, LPWSTR )>( GetProcAddress( hInst, "SHGetFolderPathW" ) );

	if ( NULL == pfnSHGetFolderPath )
	{
		// function not available!
		ASSERT( 0 );
		FreeLibrary( hInst ); // <-- here
		return FALSE;
	}


	// call it
	HRESULT hRet = pfnSHGetFolderPath( NULL, iType, NULL, 0, szPath.GetBufferSetLength( _MAX_PATH ) );
	//     HRESULT hRet = pfnSHGetFolderPathA( NULL, iType, NULL, 0, buff );
	szPath.ReleaseBuffer();
	FreeLibrary( hInst ); // <-- and here
	return TRUE;
	return 0;
}



void LaunchySmarts::getCatalogLock(void)
{
	WaitForSingleObject(hMutex, INFINITE);
}

void LaunchySmarts::releaseCatalogLock(void)
{
	ReleaseMutex(hMutex);
}


void LaunchySmarts::getStrings(CStringArray& strings)
{
	for(map<TCHAR, CharSectionPtr>::iterator it = charMap.begin(); it != charMap.end(); ++it) {
		for(vector<FileRecordPtr>::iterator jt = it->second->begin(); jt != it->second->end(); ++jt) {
			strings.Add(jt->get()->fullPath);
		}
	}
}

void LaunchySmarts::archiveCatalog(CString path, Plugin* plugins)
{
	map<CString, bool> used;
	CArray<ArchiveType> files;

	//I DON'T SEEM TO PROPERLY RECORD PLUGIN RECORDS
	for(map<TCHAR, CharSectionPtr>::iterator it = charMap.begin(); it != charMap.end(); ++it) {
		for(vector<FileRecordPtr>::iterator jt = it->second->begin(); jt != it->second->end(); ++jt) {
			if (used.count(jt->get()->fullPath) == 0) {
				used[jt->get()->fullPath] = true;
				int nametag = -1;	
				if (jt->get()->owner != -1) 
					 nametag = plugins->GetPluginNameTag(jt->get()->owner);
				ArchiveType at(jt->get()->fullPath, jt->get()->usage, nametag);
				files.Add(at);
			}
		}
	}

	CFile theFile;

	CString dir,dir2;
	dir.Format(_T("%slaunchy.db"), path);
	dir2.Format(_T("%slaunchy2.db"),path);
	if (!theFile.Open(dir2, CFile::modeWrite | CFile::modeCreate)) {
		return;
	}

	CArchiveExt archive(&theFile, CArchive::store, 4096, NULL, _T(""), TRUE);
	CDiskObject disk;

	try {
		files.Serialize(archive);
		archive.Close();
		theFile.Close();
		disk.RemoveFile(dir);
		disk.RenameFile(dir2,dir);
	} catch (...) {
		archive.Close();
		theFile.Close();
		return;
	}
}


CString LaunchySmarts::GetMatchPath(int sel)
{
	if(matches.size() >= (uint) (sel+1)) {
		return matches[sel]->fullPath;
	} else {
		return _T("");
	}
}
