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
#include "Launcher.h"

#include "boost/smart_ptr.hpp"
using namespace boost;

typedef shared_ptr<FileRecord> FileRecordPtr;

class FileRecord : public CObject {
public:
	DECLARE_SERIAL( FileRecord )
	FileRecord(void);
	~FileRecord(void);
	CString fullPath;
	CString croppedName;
	CString lowName;
	int usage;
	bool isHistory;
//	Launcher* launcher;
	
	void set(CString p, CString type, Launcher* l, int u = 0);

//	bool isHistory;
	void setUsage(int x);
	void Serialize( CArchive& archive );

    FileRecord( const FileRecord &s ) {  // copy ctor 
		fullPath = s.fullPath;
		croppedName = s.croppedName;
		lowName = s.lowName;
		usage = s.usage;
	}
	FileRecord& operator=( const FileRecord &s )  {// assignment operator
		fullPath = s.fullPath;
		croppedName = s.croppedName;
		lowName = s.lowName;
		usage = s.usage;
		return *this;
	}

	CString GetDirectory(void);
};
