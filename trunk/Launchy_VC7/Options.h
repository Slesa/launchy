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

class Options
{
public:
	shared_ptr<CIniFile> ini;
	vector<shared_ptr<Skin> > skins;
//	CArray<CString> Types;
//	CArray<CString> Directories;

	vector<CString> Types;
	vector<CString> Directories;

	shared_ptr<Skin> skin;
	CString skinName;

	CString userDir;
	CTime installTime;
	bool firstRun;
	int listLength;
	bool stickyWindow;
	int posX;
	int posY;
	int ver;
//	CList<shared_ptr<LaunchyDir> > dirs;
	vector<shared_ptr<LaunchyDir> > dirs;
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
};
