// Copyright 2009 Fabian Hofsaess
#include <QString>
#include "winfiles.h"
#include "fhoenv.h"

// Launchy already implements expandEnvironmentVars but it's difficult to reuse in plugins
QString FhoEnv::expand(QString txt) {
	QString result;

	DWORD size = ExpandEnvironmentStrings((LPCWSTR)txt.utf16(), NULL, 0);
	if (size > 0)
	{
		TCHAR* buffer = new TCHAR[size];
		ExpandEnvironmentStrings((LPCWSTR)txt.utf16(), buffer, size);
        result = QString::fromWCharArray(buffer);
		delete[] buffer;
	}

	return result;
}
