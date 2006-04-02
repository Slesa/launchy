#pragma once
#include "Launcher.h"

#include <boost/smart_ptr.hpp>
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
//	Launcher* launcher;
	
	void set(CString p, CString type, Launcher* l, int u = 0);

	bool isHistory;
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

};
