// Foxy.h : main header file for the Foxy DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols


// CFoxyApp
// See Foxy.cpp for the implementation of this class
//

class CFoxyApp : public CWinApp
{
public:
	CFoxyApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
