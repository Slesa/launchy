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
{
	CString dir;
	LaunchySmarts::GetShellDir(CSIDL_LOCAL_APPDATA, dir);
	dir += "\\launchy.ini";
	ini->SetPath(dir.GetBuffer());
	ini->ReadFile();
	Load();
}



Options::~Options(void)
{
	Store();
}

void Options::Load(void)
{
	posX = ini->GetValueI(L"Position", L"pos_x");
	posY = ini->GetValueI(L"Position", L"pos_y");
}

void Options::Store(void)
{
	CWinApp* pApp = AfxGetApp();
	if (pApp == NULL) return;
	RECT location;
	pApp->GetMainWnd()->GetWindowRect(&location);

	ini->SetValueI(L"Position", L"pos_x", location.left);
	ini->SetValueI(L"Position", L"pos_y", location.top);

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
