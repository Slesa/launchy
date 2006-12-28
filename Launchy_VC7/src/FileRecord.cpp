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
#include "FileRecord.h"
#include "Launchy.h"
#include ".\filerecord.h"


IMPLEMENT_SERIAL( FileRecord, CObject, 1 )

void FileRecord::Serialize( CArchive& archive )
{
	// call base class function first
	// base class is CObject in this case
	CObject::Serialize( archive );

	// now do the stuff for our specific class
	if( archive.IsStoring() )
		archive << fullPath << croppedName << lowName << usage;
	else
		archive >> fullPath >> croppedName >> lowName >> usage;
}

FileRecord::FileRecord(void)	 : isHistory(false)
{
	owner = -1;
}

FileRecord::~FileRecord(void)
{
}

void FileRecord::set(CString p, CString type, Launcher* l, int u /* = 0 */)
{
/*	if (type == _T(".directory")) {
		p = p.Mid(0, p.GetLength() - type.GetLength());
		int x = 3;
		x = x + 1;
	}
	*/
	usage = u;
	CString fileName;
	fullPath = p;

	int ind = p.ReverseFind(_T('\\'));
	if (ind != -1) {
		fileName = p.Mid(ind+1);
	}

	croppedName = fileName.Mid(0, fileName.GetLength() - type.GetLength());

	lowName = croppedName;
	lowName.MakeLower();

	owner = -1;
}


void FileRecord::setUsage(int x)
{
	usage = x;
}

CString FileRecord::GetDirectory(void)
{
	CString dir;
	int ind = fullPath.ReverseFind(_T('\\'));
	if (ind != -1) {
		return fullPath.Mid(0, ind + 1);
	} 
	return _T("");
}
