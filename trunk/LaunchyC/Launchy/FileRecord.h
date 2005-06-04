#pragma once
#include "Launcher.h"

class FileRecord
{
public:
	FileRecord(void);
	~FileRecord(void);
	CString fullPath;
	CString fileType;
	CString fileName;
	CString dirPath;
	CString croppedName;
	CString lowName;
	Launcher* launcher;
	
	void set(CString p, CString type, Launcher* l);
	
	bool operator< (const FileRecord & other) const;
	bool operator> (const FileRecord & other) const;
};
