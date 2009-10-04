// Copyright 2009 Fabian Hofsaess

#ifndef FHORES_H
#define FHORES_H

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

class FhoRes {

	public:

		static QString getResourceString(QString &resourceIdentifier);
		static QString getResourceString(QString &resourceName, int resourceId);
		static QString getResourceString(HINSTANCE hLib, int resourceId);

};

#endif