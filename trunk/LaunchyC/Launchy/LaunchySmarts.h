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
#include "ExeLauncher.h"
#include "DiskObject.h"
#include "afxtempl.h"
#include "FileRecord.h"
#include "QArray.h"



class LaunchySmarts
{
public:
	LaunchySmarts(void);
	~LaunchySmarts(void);
	void LoadCatalog(void);
protected:
	void ScanStartMenu(void);
	ExeLauncher exeLauncher;
	CDiskObject disk;
public:
	void ScanDir(CString path, CString extension, Launcher* launcher);

protected:
	CQArray<FileRecordPtr, FileRecordPtr> catalog;
	CQArray<FileRecordPtr, FileRecordPtr> matches;
public:
	void Update(CString txt);
	void FindMatches(CString txt);
	BOOL Match(FileRecordPtr record, CString txt);
	void Launch(void);
protected:
	void RemoveDuplicates(void);
public:
	static BOOL GetShellDir(int iType, CString& szPath);
};