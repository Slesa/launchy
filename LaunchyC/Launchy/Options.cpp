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

Options::Options(void)
{

	Load();
}



Options::~Options(void)
{
	Store();
	while (!dirs.IsEmpty())
		delete dirs.RemoveHead();
}

void Options::Load(void)
{
	CWinApp* pApp = AfxGetApp();
	posX = (int)pApp->GetProfileInt(_T("Settings"),_T("posX"),-1);
	posY = (int)pApp->GetProfileInt(_T("Settings"),_T("posY"),-1);
}

void Options::Store(void)
{
	CWinApp* pApp = AfxGetApp();
	RECT location;
	pApp->GetMainWnd()->GetWindowRect(&location);

	posX = (int)pApp->WriteProfileInt(_T("Settings"),_T("posX"),location.left);
	posY = (int)pApp->WriteProfileInt(_T("Settings"),_T("posY"),location.top);	
}

