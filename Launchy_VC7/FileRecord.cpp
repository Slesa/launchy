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

FileRecord::FileRecord(void)
: isHistory(false)
{
}

FileRecord::~FileRecord(void)
{
}

void FileRecord::set(CString p, CString type, Launcher* l, int u /* = 0 */)
{
	usage = u;
	CString fileName;
	fullPath = p;

	int ind = p.ReverseFind(_T('\\'));
	if (ind != -1) {
		fileName = p.Mid(ind+1);
	} else {
		fileName = p;
	}

	croppedName = fileName.Mid(0, fileName.GetLength() - type.GetLength());

	lowName = croppedName;
	lowName.MakeLower();
}


void FileRecord::setUsage(int x)
{
	usage = x;
}
