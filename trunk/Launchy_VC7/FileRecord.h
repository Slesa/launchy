#pragma once
#include "Launcher.h"

#include <boost/smart_ptr.hpp>
using namespace boost;

//typedef shared_ptr<FileRecord> FileRecordPtr;
typedef FileRecord* FileRecordPtr;

class FileRecord
{
public:
	FileRecord(void);
	~FileRecord(void);
	CString fullPath;
	CString croppedName;
	CString lowName;
	Launcher* launcher;
	
	void set(CString p, CString type, Launcher* l);

	bool isHistory;
};
