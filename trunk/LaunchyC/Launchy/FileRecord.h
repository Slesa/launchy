#pragma once
#include "Launcher.h"

#include <boost/smart_ptr.hpp>
using namespace boost;

typedef shared_ptr<FileRecord> FileRecordPtr;

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
	int matchValue;
	
	void set(CString p, CString type, Launcher* l);
	
	bool operator< (const FileRecord & other) const;
	bool operator> (const FileRecord & other) const;
	bool isHistory;
};
