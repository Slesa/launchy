// Copyright 2009 Fabian Hofsaess

#ifndef FHOREG_H
#define FHOREG_H

#include <QString>

#ifdef Q_WS_WIN
	#define VC_EXTRALEAN
	#ifndef WINVER
		#define WINVER 0x05100
		#define _WIN32_WINNT 0x0510	
		#define _WIN32_WINDOWS 0x0510 
		#define _WIN32_IE 0x0600
	#endif
#endif

#include <windows.h>

// implement a convenient class instead of an aggregation of static semi-convenient methods!

class FhoReg {
	private:
		static const DWORD maxSize = 256;

	public:
		static HKEY OpenKey(HKEY baseKey, QString &subKeyName, DWORD options);
		static void CloseKey(HKEY key);

		static QString GetKeyValue(HKEY key, QString &valueName);
		static QString GetKeyValue(HKEY parentKey, QString &parentSubKeyName, QString &valueName);
		static QString GetKeyDefaultValue(HKEY key);
		static QString GetKeyDefaultValue(HKEY parentKey, QString &parentSubKeyName);

		static QStringList* EnumValues(HKEY parentKey, QString &parentSubKeyName);
		static QStringList* EnumSubKeys(HKEY key);
		static QStringList* EnumSubKeys(HKEY parentKey, QString &parentSubKeyName);

};

#endif