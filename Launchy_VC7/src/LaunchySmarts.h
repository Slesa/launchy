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

#pragma once
#include "stdafx.h"
#include "ExeLauncher.h"
#include "DiskObject.h"
#include "afxtempl.h"
#include "FileRecord.h"
#include "UseShGetFileInfo.h"
#include "Plugin.h"
#include <map>
#include <vector>

using namespace std;

#include "boost/smart_ptr.hpp"
using namespace boost;

typedef shared_ptr<vector<FileRecordPtr> > CharSectionPtr;

class LaunchySmarts;
#include "Options.h"


struct ScanBundle {
	Options* ops;
	LaunchySmarts* smarts;
	Plugin* plugins;
	map<TCHAR, int> charUsage;
	map<TCHAR, CharSectionPtr> charMap;
	int catFiles;
	HWND dlg;
};




class LaunchySmarts
{


public:
	int catFiles;
	LaunchySmarts(void);
	~LaunchySmarts(void);
	void LoadCatalog(void);
protected:
	//	static UINT ScanStartMenu(LPVOID pParam);

	CDiskObject disk;
public:
	map<TCHAR, int> charUsage;
	map<TCHAR, CharSectionPtr> charMap;
	//	static void ScanDir(CString path, Launcher* launcher,map<CString, bool>& catalog, map<CString,bool>& typeMap);

	vector<FileRecordPtr> matches;
protected:

public:
	void Update(CString txt, bool UpdateDropdown = true, CString oneTimeHistory = _T(""));
	void FindMatches(CString txt);
	BOOL Match(FileRecordPtr record, CString txt);
	ExeLauncher exeLauncher;
	void Launch(void);
private:

protected:
	void RemoveDuplicates(void);
public:
	static BOOL GetShellDir(int iType, CString& szPath);
	void getCatalogLock(void);
	void releaseCatalogLock(void);
	HANDLE hMutex;
	void getStrings(CStringArray& strings);
	void LoadFirstTime();
	void archiveCatalog(CString path, Plugin*);
	CString lastUpdateTxt;
	CString GetMatchPath(int sel);
};
