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
#include "Options.h"
#include "LaunchySmarts.h"


Options::Options(void) : ini(new CIniFile())
, vkey(0)
, mod_key(0)
{
	CString dir;
	LaunchySmarts::GetShellDir(CSIDL_LOCAL_APPDATA, dir);
	dir += "\\launchy.ini";
	ini->SetPath(dir.GetBuffer());
	ini->ReadFile();
	ParseIni();
	LoadSkins();
}



Options::~Options(void)
{
	Store();
}

void Options::ParseIni(void)
{
	posX = ini->GetValueI(L"Position", L"pos_x");
	posY = ini->GetValueI(L"Position", L"pos_y");

	mod_key =  ini->GetValueI(L"Hotkey", L"mod_key", MOD_ALT);
	vkey =  ini->GetValueI(L"Hotkey", L"vkey", VK_SPACE);

	skinName = ini->GetValue(L"Skin", L"name", L"Blue Trim").c_str();
}

void Options::Store(void)
{
	CWinApp* pApp = AfxGetApp();
	if (pApp == NULL) return;
	RECT location;
	pApp->GetMainWnd()->GetWindowRect(&location);

	ini->SetValueI(L"Position", L"pos_x", location.left);
	ini->SetValueI(L"Position", L"pos_y", location.top);

	ini->SetValueI(L"Hotkey", L"mod_key", mod_key);
	ini->SetValueI(L"Hotkey", L"vkey", vkey);

	ini->SetValue(L"Skin", L"name", skinName.GetBuffer());
	ini->WriteFile();
}

void Options::Associate(CString entry, CString destination)
{
	ini->SetValue(L"Associations", std::wstring(entry.GetBuffer()), std::wstring(destination.GetBuffer()));
}


CString Options::GetAssociation(CString query)
{
	wstring res = ini->GetValue(L"Associations", std::wstring(query.GetBuffer()));

	if (res != L"") {
		CString x = CString(res.c_str());
		return x;
	}
	return _T("");
}

void Options::LoadSkins(void)
{
	wchar_t buffer[_MAX_PATH];
	
	/* Get the current working directory: */
	_wgetcwd( buffer, _MAX_PATH );

	CString dir = buffer;
	dir += "\\Skins\\";


	CDiskObject disk;
	CStringArray skinDirs;
	disk.EnumAllDirectories(dir, skinDirs);

	int count = skinDirs.GetCount();
	for(int i = 0; i < count; i++) {
		shared_ptr<Skin> x(new Skin(skinDirs[i]));
		skins.AddTail(x);
		if (x->name == skinName) {
			skin = x;
		}
	}


}
