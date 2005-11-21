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
#include <boost/shared_ptr.hpp>
#include "Skin.h"

using namespace boost;

class Options
{
public:
	shared_ptr<CIniFile> ini;
	CList<shared_ptr<Skin> > skins;
	shared_ptr<Skin> skin;
	CString skinName;

	int posX;
	int posY;
	CList<shared_ptr<LaunchyDir> > dirs;

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
};
