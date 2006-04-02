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
	ArchiveType(CString str, int u): name(str), usage(u) {}
	CString name;
	int usage;
	void Serialize( CArchive& archive )
	{
		// call base class function first
		// base class is CObject in this case
		CObject::Serialize( archive );

		// now do the stuff for our specific class
		if( archive.IsStoring() )
			archive << name << usage;
		else
			archive >> name >> usage;
	}
    ArchiveType( const ArchiveType &s ) {  // copy ctor 
			name = s.name;
		  usage = s.usage;
	}
	ArchiveType& operator=( const ArchiveType &s )  {// assignment operator
			name = s.name;
		  usage = s.usage;
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

	int localFind = a->lowName.Find(searchTxt);
	int otherFind = b->lowName.Find(searchTxt);
	int localLen = a->lowName.GetLength();
	int otherLen = b->lowName.GetLength();

	if(a->usage > b->usage)
		return true;
	if (a->usage < b->usage)
		return false;
	if (localFind > -1 && otherFind == -1)
		return true;
	if (localFind == -1 && otherFind > -1)
		return false;
	if (localLen < otherLen)
		return true;
	if (localLen > otherLen)
		return false;

	return false;
}


LaunchySmarts::LaunchySmarts(void)
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
	this->archiveCatalog();
	releaseCatalogLock();
}


int findCount(LaunchySmarts* smarts, CString word) {
	if (smarts->charMap.count(word[0]) == 0)
		return 0;
	for(uint i = 0; i < smarts->charMap[word[0]]->size(); i++) {
		if (word == smarts->charMap[word[0]]->at(i)->lowName) {
			return smarts->charMap[word[0]]->at(i)->usage;
		}
	}
	return 0;
}



void ScanFiles(CArray<ArchiveType>& in, ScanBundle* bun, CArray<ArchiveType>& out)
{
	map<CString,bool> catalog;
	map<CString, bool> typeMap;
	for(uint i = 0; i < bun->ops->Types.size(); i++) {
		typeMap[bun->ops->Types[i]] = true;
	}

	// Force "lnk" files
	typeMap[_T(".lnk")] = true;


	CString tmps;
	Options* ops = bun->ops;

	CMap<TCHAR, TCHAR&, bool, bool&> added;

	INT_PTR count = in.GetCount();

	for(int i = 0; i < count; i++) {
		tmps = in[i].name.Mid(in[i].name.GetLength()-4,4);
		tmps.MakeLower();
		if (typeMap[tmps] == false) continue;
		FileRecordPtr rec(new FileRecord());
		rec->set(in[i].name, tmps, &bun->smarts->exeLauncher);

		if (in[i].usage != -1)
			rec->setUsage(in[i].usage);
		else
			rec->setUsage(findCount(bun->smarts, rec->lowName));

		ArchiveType at(in[i].name, rec->usage);
		out.Add(at);
		if (catalog[rec->lowName] == true) continue;
		catalog[rec->lowName] = true;
		bun->catFiles += 1;
		added.RemoveAll();
		for(int i = 0; i < rec->lowName.GetLength( ); i++) {
			TCHAR c = rec->lowName[i];
			bun->charUsage[c] += 1;

			if (bun->charMap[c] == NULL) {
				bun->charMap[c].reset(new vector<FileRecordPtr>());	
			}
			if (added[c] == false) {
				bun->charMap[c]->push_back(rec);
				added[c] = true;
			}
		}
	}
}


UINT ScanStartMenu(LPVOID pParam)
{
	ScanBundle* bun = (ScanBundle*) pParam;

	Options* ops = bun->ops;
	CString myMenu, allMenus;

	if (FALSE == bun->smarts->GetShellDir(CSIDL_COMMON_STARTMENU, allMenus))
		return 0;
	if (FALSE == bun->smarts->GetShellDir(CSIDL_STARTMENU, myMenu))
		return 0;

	CStringArray files;
	CArray<ArchiveType> input;
	CArray<ArchiveType> smaller;
	CStringArray tmpFiles;
	files.SetSize(1024);
	tmpFiles.SetSize(1024);


	CDiskObject disk;
	disk.EnumAllFiles(myMenu, files);
	disk.EnumAllFiles(allMenus, tmpFiles);
	files.Append(tmpFiles);
	tmpFiles.RemoveAll();

	for(uint i = 0; i < ops->Directories.size(); i++) {
		disk.EnumAllFiles(ops->Directories[i], tmpFiles);
		files.Append(tmpFiles);
		tmpFiles.RemoveAll();
	}

	for(int i = 0; i < files.GetSize(); i++) {
		ArchiveType at(files[i], -1);
		input.Add(at);
	}

	ScanFiles(input, bun, smaller);


	// Now replace the catalog files
	bun->smarts->getCatalogLock();

	bun->smarts->charMap = bun->charMap;
	bun->smarts->charUsage = bun->charUsage;
	bun->smarts->catFiles = bun->catFiles;

	bun->smarts->releaseCatalogLock();

	delete bun;

	return 0;
}

void LaunchySmarts::LoadCatalog(void)
{
	ScanBundle* bundle = new ScanBundle();
	bundle->smarts = this;
	bundle->ops = ((CLaunchyDlg*)AfxGetMainWnd())->options.get();
	bundle->catFiles = 0;
	AfxBeginThread(ScanStartMenu, bundle);
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
	TCHAR name[256];
	DWORD size = 256;
	GetUserName(name, &size);
	CString dir;
	dir.Format(_T("Users\\ %s\\"), name);
//	LaunchySmarts::GetShellDir(CSIDL_LOCAL_APPDATA, dir);
//	dir += _T("\\Launchy");
	dir += _T("\\launchy.db");

	// If the version is less than 0.91, we can't use the old
	// database
	// If there is no database, we can't use it either
	if (!theFile.Open(dir, CFile::modeRead) || ((CLaunchyDlg*)AfxGetMainWnd())->options->ver < 91) {
			LoadCatalog();
			return;
	}

	CArchiveExt archive(&theFile, CArchive::load, 4096, NULL, _T(""), TRUE);

	CArray<ArchiveType> archtypes;
	CArray<ArchiveType> smaller;

	archtypes.Serialize(archive);

	archive.Close();
	theFile.Close();


	ScanBundle* bundle = new ScanBundle();
	bundle->smarts = this;
	bundle->ops = ((CLaunchyDlg*)AfxGetMainWnd())->options.get();
	bundle->catFiles = 0;
	ScanFiles(archtypes, bundle, smaller);

	// Now replace the catalog files
	bundle->smarts->getCatalogLock();

	bundle->smarts->charMap = bundle->charMap;
	bundle->smarts->charUsage = bundle->charUsage;
	bundle->smarts->catFiles = bundle->catFiles;

	bundle->smarts->releaseCatalogLock();

	delete bundle;
}



void LaunchySmarts::Update(CString txt, bool UpdateDropdown)
{
	CLaunchyDlg* pDlg = (CLaunchyDlg*) AfxGetMainWnd();
	if (pDlg == NULL)
		return;

	if (txt == "") return;

	CString history = pDlg->options->GetAssociation(txt);

	matches.clear();
	FindMatches(txt);

	// Set the preferred bit for the history match
	size_t count = matches.size();
	for(size_t i = 0; i < count; i++) {
		if (matches[i]->croppedName == history) {
			matches[i]->isHistory = true;
		}
	}

	sort(matches.begin(), matches.end(), less_than);

	// Unset the preferred bit for the history match
	if (count > 0)
		matches[0]->isHistory = false;

	if (matches.size() > 0) {
		HICON hNew = IconInfo.GetIconHandleNoOverlay(matches[0]->fullPath, false);

		HICON h = pDlg->IconPreview.SetIcon(hNew);
		if (h != hNew) {
			DestroyIcon(h);
		}
		pDlg->Preview.SetWindowText(matches[0]->croppedName);
	} else {
		pDlg->Preview.SetWindowText(_T(""));
	}

	if (UpdateDropdown) {
		
	pDlg->InputBox.m_listbox.ResetContent();
		size_t size = matches.size();
		for(size_t i = 0; i < size && i < 10; i++) {
			CString blah = matches[i]->croppedName;
			pDlg->InputBox.AddString(matches[i]->croppedName);
		}
	}

}

void LaunchySmarts::FindMatches(CString txt)
{
	getCatalogLock();
	txt.MakeLower();

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

	if (charMap[mostInfo] != NULL) {
		size_t count = charMap[mostInfo]->size();
		for(size_t i = 0; i < count; i++) {
			if (Match(charMap[mostInfo]->at(i), txt)) {
				matches.push_back(charMap[mostInfo]->at(i));
			}
		}
	}

	releaseCatalogLock();
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
	if(matches.size() > 0) {
		matches[0]->setUsage(matches[0]->usage + 1);
		exeLauncher.Run(matches[0]);
//		matches[0]->launcher->Run(matches[0]);
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

void LaunchySmarts::archiveCatalog(void)
{
	map<CString, bool> used;
	CArray<ArchiveType> files;

	for(map<TCHAR, CharSectionPtr>::iterator it = charMap.begin(); it != charMap.end(); ++it) {
		for(vector<FileRecordPtr>::iterator jt = it->second->begin(); jt != it->second->end(); ++jt) {
			if (used.count(jt->get()->fullPath) == 0) {
				used[jt->get()->fullPath] = true;
				ArchiveType at(jt->get()->fullPath, jt->get()->usage);
				files.Add(at);
			}
		}
	}


	CFile theFile;
	TCHAR name[256];
	DWORD size = 256;
	GetUserName(name, &size);
	CString dir;
	dir.Format(_T("Users\\ %s\\"), name);

	dir += _T("\\launchy.db");

	if (!theFile.Open(dir, CFile::modeWrite | CFile::modeCreate)) {
		return;
	}

	CArchiveExt archive(&theFile, CArchive::store, 4096, NULL, _T(""), TRUE);

	files.Serialize(archive);

	archive.Close();
	theFile.Close();
}
