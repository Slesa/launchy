#include "StdAfx.h"
#include "FileRecord.h"
#include "Launchy.h"

FileRecord::FileRecord(void)
{
}

FileRecord::~FileRecord(void)
{
}

void FileRecord::set(CString p, CString type, Launcher* l)
{
	launcher = l;
	fullPath = p;
	if (type[0] == '*')
		fileType = type.Mid(1);
	else
		fileType = type;

	int ind = p.ReverseFind(_T('\\'));
	if (ind != -1) {
		fileName = p.Mid(ind+1);
		dirPath = p.Mid(0,ind);
	} else {
		fileName = p;
		dirPath = p;
	}

	lowName = fileName;
	lowName.MakeLower();

	ind = fileName.Find(fileType);
	if (ind != -1) {
		croppedName = fileName.Mid(0, ind);
	} else {
		croppedName = fileName;
	}
	
}


bool FileRecord::operator<(const FileRecord & other) const 
{
	int localFind = lowName.Find(searchTxt);
	int otherFind = other.lowName.Find(searchTxt);
	int localLen = lowName.GetLength();
	int otherLen = other.lowName.GetLength();
	
	if (localFind > -1 && otherFind == -1)
		return 1;
	if (localFind == -1 && otherFind > -1)
		return 0;
	if (localLen < otherLen)
		return 1;
	if (localLen > otherLen)
		return 0;

	return 0;
}

bool FileRecord::operator>(const FileRecord & other) const
{
	int localFind = lowName.Find(searchTxt);
	int otherFind = other.lowName.Find(searchTxt);
	int localLen = lowName.GetLength();
	int otherLen = other.lowName.GetLength();
	
	if (localFind > -1 && otherFind == -1)
		return 0;
	if (localFind == -1 && otherFind > -1)
		return 1;
	if (localLen < otherLen)
		return 0;
	if (localLen > otherLen)
		return 1;

	return 0;
}