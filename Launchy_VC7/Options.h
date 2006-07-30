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


#include "LaunchyDir.h"
#include "iniFile.h"
#include "boost/shared_ptr.hpp"
#include "Skin.h"

#include <vector>

using namespace std;

using namespace boost;

class DirOptions {
public:
	CString dir;
	vector<CString> types;

	DirOptions(CString s) {
		dir = s;
	}
	DirOptions() {
		dir = _T("");
	}
	DirOptions( const DirOptions &s ) {  // copy ctor 
		dir = s.dir;
		types = s.types;
	}
	DirOptions& operator=( const DirOptions &s )  {// assignment operator
		dir = s.dir;
		types = s.types;
		return *this;
	}
};

class Options
{
public:

	shared_ptr<CIniFile> ini;
	vector<shared_ptr<Skin> > skins;
	//	CArray<CString> Types;
	//	CArray<CString> Directories;


	shared_ptr<Skin> skin;
	CString skinName;

	bool usbmode;

	CTime installTime;
	bool firstRun;
	int listLength;
	bool stickyWindow;
	int posX;
	int posY;
	int ver;
	//	CList<shared_ptr<LaunchyDir> > dirs;

private:
	HANDLE hMutex;
	bool Indexing;
	CString dataPath;
	vector<CString> Types;
	vector<DirOptions> Directories;

public:
	Options(void);
	~Options(void);
	void ParseIni(void);
	void Store(void);
	void Associate(CString entry, CString destination); 	 
	CString GetAssociation(CString query);
public:
	UINT vkey;
public:
	UINT mod_key;
public:
	void LoadSkins(void);
	void UpgradeCleanup(void);
	void SetSkin(CString skinName);
	void changeUsbMode(bool toUSB);

	void getLock();
	void relLock();

	CString get_dataPath() { getLock(); CString ret = dataPath; relLock(); return ret; }
	void set_dataPath(CString a1) { getLock(); dataPath = a1; relLock(); }

	vector<CString> get_Types() { getLock(); vector<CString> ret = Types; relLock(); return ret; }
	void set_Types (vector<CString> a1) { getLock(); Types = a1; relLock(); }

	vector<DirOptions> get_Directories() { getLock(); vector<DirOptions> ret = Directories; relLock(); return ret; }
	void set_Directories (vector<DirOptions> a1) { getLock(); Directories = a1; relLock(); }

	bool get_Indexing() { getLock(); bool ret = Indexing; relLock(); return ret; }
	void set_Indexing(bool a1) { getLock(); Indexing = a1; relLock(); }



};
