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


bool less_than(const shared_ptr<FileRecord> a, const shared_ptr<FileRecord> b)
{
	if (a->isHistory) { return true; } 
	if (b->isHistory) { return false; }

	int localFind = a->lowName.Find(searchTxt);
	int otherFind = b->lowName.Find(searchTxt);
	int localLen = a->lowName.GetLength();
	int otherLen = b->lowName.GetLength();
	
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
	catFiles = 0;
	LoadCatalog();
}

LaunchySmarts::~LaunchySmarts(void)
{
}

void LaunchySmarts::LoadCatalog(void)
{
	catFiles = 0;
	charUsage.clear();
	charMap.clear();
	ScanStartMenu();
}

void LaunchySmarts::ScanStartMenu(void)
{
	
	shared_ptr<Options> ops = ((CLaunchyDlg*)AfxGetMainWnd())->options;
	CString myMenu, allMenus;
	if (FALSE == GetShellDir(CSIDL_COMMON_STARTMENU, allMenus))
		return;
	if (FALSE == GetShellDir(CSIDL_STARTMENU, myMenu))
		return;

	map<CString,bool> catalog;
	map<CString, bool> typeMap;
	for(int i = 0; i < ops->Types.size(); i++) {
		typeMap[ops->Types[i]] = true;
	}

	// Force "lnk" files
	typeMap[_T("lnk")] = true;

	ScanDir(myMenu, &exeLauncher, catalog, typeMap);
	ScanDir(allMenus, &exeLauncher, catalog, typeMap);

	for(int i = 0; i < ops->Directories.size(); i++) {
			ScanDir(ops->Directories[i], &exeLauncher, catalog, typeMap);
	}
//	ScanDir(_T("C:\\Documents and Settings\\karlinjf\\My Documents\\My Music"), _T("*.mp3"), &exeLauncher);
}

void LaunchySmarts::ScanDir(CString path, Launcher* launcher, map<CString, bool>& catalog, map<CString,bool>& typeMap)
{
	CStringArray files;
	CString tmps;
	shared_ptr<Options> ops = ((CLaunchyDlg*)AfxGetMainWnd())->options;



	//	disk.EnumAllFilesWithFilter(extension, path, files);
	disk.EnumAllFiles(path, files);




	CMap<char, char&, bool, bool&> added;

	int count = files.GetCount();

	for(int i = 0; i < count; i++) {
		tmps = files[i].Mid(files[i].GetLength()-4,4);
		tmps.MakeLower();
		if (typeMap[tmps] == false) continue;
		FileRecordPtr rec(new FileRecord());
		rec->set(files[i], tmps, launcher);

		if (catalog[rec->lowName] == true) continue;
		catalog[rec->lowName] = true;
		catFiles += 1;
		added.RemoveAll();
		for(int i = 0; i < rec->lowName.GetLength( ); i++) {
			char c = rec->lowName[i];
			charUsage[c] += 1;

			if (charMap[c] == NULL) {
				charMap[c].reset(new vector<FileRecordPtr>());	
			}
			if (added[c] == false) {
				charMap[c]->push_back(rec);
				added[c] = true;
			}
		}
	}
}



void LaunchySmarts::Update(CString txt)
{
	CLaunchyDlg* pDlg = (CLaunchyDlg*) AfxGetMainWnd();
	if (pDlg == NULL)
		return;

	if (txt == "") return;

	CString history = pDlg->options->GetAssociation(txt);

	matches.clear();
	pDlg->InputBox.m_listbox.ResetContent();
	FindMatches(txt);

	// Set the preferred bit for the history match
	int count = matches.size();
	for(int i = 0; i < count; i++) {
		if (matches[i]->croppedName == history) {
			matches[i]->isHistory = true;
		}
	}

	sort(matches.begin(), matches.end(), less_than);

	// Unset the preferred bit for the history match
	if (count > 0)
		matches[0]->isHistory = false;


	if (matches.size() > 0) {
		pDlg->Preview.SetWindowText(matches[0]->croppedName);
	} else {
		pDlg->Preview.SetWindowText(_T(""));
	}

	
	int size = matches.size();
	for(int i = 0; i < size && i < 10; i++) {
		pDlg->InputBox.AddString(matches[i]->croppedName);
	}

}

void LaunchySmarts::FindMatches(CString txt)
{
	txt.MakeLower();

	char mostInfo = -1;
	// Find the character with the most amount of information
	for(int i = 0; i < txt.GetLength(); i++) {
		char c = txt[i];
		if (charUsage[c] < charUsage[mostInfo] || mostInfo == -1) {
			mostInfo = c;
		}
	}

	if (charMap[mostInfo] != NULL) {
		int count = charMap[mostInfo]->size();
		for(int i = 0; i < count; i++) {
			if (Match(charMap[mostInfo]->at(i), txt)) {
				matches.push_back(charMap[mostInfo]->at(i));
			}
		}
	}

	else {
		AfxMessageBox(_T("I shouldn't get here"));
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
	if(matches.size() > 0) {
		matches[0]->launcher->Run(matches[0]);
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
     szPath.ReleaseBuffer();
     FreeLibrary( hInst ); // <-- and here
     return TRUE;
	return 0;
}
