#include "StdAfx.h"
#include "FileRecord.h"
#include "Launchy.h"

FileRecord::FileRecord(void)
: isHistory(false)
{
}

FileRecord::~FileRecord(void)
{
}

void FileRecord::set(CString p, CString type, Launcher* l)
{
	CString fileName, fileType;
	launcher = l;
	fullPath = p;
	if (type[0] == '*')
		fileType = type.Mid(1);
	else
		fileType = type;

	int ind = p.ReverseFind(_T('\\'));
	if (ind != -1) {
		fileName = p.Mid(ind+1);
		//dirPath = p.Mid(0,ind);
	} else {
		fileName = p;
		//dirPath = p;
	}


	ind = fileName.Find(fileType);
	if (ind != -1) {
		croppedName = fileName.Mid(0, ind);
	} else {
		croppedName = fileName;
	}

	lowName = croppedName;
	lowName.MakeLower();


}

