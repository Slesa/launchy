/*
Launchy: Application Launcher
Copyright (C) 2005  Josh Karlin

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

// Launchy.h : main header file for the PROJECT_NAME application
//

#pragma once


#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols
#include "FileRecord.h"

/*
#include <boost/spirit/phoenix.hpp>
using phoenix::arg1;
using phoenix::arg2;
using phoenix::bind;
using namespace boost::lambda;
*/
#include <boost/lambda/lambda.hpp>
using namespace std;
using namespace boost::lambda;


extern CStringArray SearchStrings;
extern int SearchPluginID;
extern CString searchTxt;
extern CString searchTxtBak;

// The filerecord that was expanded with a "tab"
extern FileRecordPtr TabbedMatch;

// CLaunchyApp:
// See Launchy.cpp for the implementation of this class
//

class CLaunchyApp : public CWinApp
{
public:
	CLaunchyApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CLaunchyApp theApp;