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

#include "StdAfx.h"
#include "LaunchySmarts.h"
#include <shlobj.h>



// Code to get shell directories
#ifndef CSIDL_WINDOWS
#define CSIDL_WINDOWS                   0x0024      // from Platform SDK
#define CSIDL_SYSTEM                    0x0025      // GetSystemDirectory()
#define CSIDL_PROGRAM_FILES             0x0026      // C:\Program Files
typedef enum {
     SHGFP_TYPE_CURRENT  = 0,   // current value for user, verify it exists
     SHGFP_TYPE_DEFAULT  = 1,   // default value, may not exist
} SHGFP_TYPE;
#endif//CSIDL_WINDOWS

BOOL GetShellDir( int iType, CString& szPath )
{
     HINSTANCE hInst = ::LoadLibrary( _T("shell32.dll") );
     if ( NULL == hInst )
     {
          ASSERT( 0 );
          return FALSE;
     }
     
//	 HRESULT (__stdcall *pfnSHGetFolderPath)( HWND, int, HANDLE, DWORD, LPTSTR );
	 HRESULT (__stdcall *pfnSHGetFolderPath)( HWND, int, HANDLE, DWORD, LPWSTR );
	 
	 
	 pfnSHGetFolderPath = reinterpret_cast<HRESULT (__stdcall *)( HWND, int, HANDLE, DWORD, LPWSTR )>( GetProcAddress( hInst, "SHGetFolderPathW" ) );

     if ( NULL == pfnSHGetFolderPath )
     {
          // function not available!
          ASSERT( 0 );
          FreeLibrary( hInst ); // <-- here
          return FALSE;
     }

     // call it
     HRESULT hRet = pfnSHGetFolderPath( NULL, iType, NULL, 0, szPath.GetBufferSetLength( _MAX_PATH ) );
     szPath.ReleaseBuffer();
     FreeLibrary( hInst ); // <-- and here
     return TRUE;
}



LaunchySmarts::LaunchySmarts(void)
{
	LoadCatalog();
}

LaunchySmarts::~LaunchySmarts(void)
{
}

void LaunchySmarts::LoadCatalog(void)
{
	//catalog.clear();

	ScanStartMenu();

}

void LaunchySmarts::ScanStartMenu(void)
{
	CString myMenu, allMenus;
	if (FALSE == GetShellDir(CSIDL_COMMON_STARTMENU, allMenus))
		return;
	if (FALSE == GetShellDir(CSIDL_STARTMENU, myMenu))
		return;
	
	ScanDir(myMenu, _T("*.lnk"), &exeLauncher);
	ScanDir(allMenus, _T("*.lnk"), &exeLauncher);
}

void LaunchySmarts::ScanDir(CString path, CString extension, Launcher* launcher)
{
	CStringArray files;
	disk.EnumAllFilesWithFilter(extension, path, files);

	for(int i = 0; i < files.GetCount(); i++) {
//		CreateRecord(files[i]);
	}
}
