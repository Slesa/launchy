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


////////////////////////////////////////////////////////////
//                                                        //
// ClassName.cpp: implementation of the CClassName class. //
//                                                        //
////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "UseShGetFileInfo.h"
#include "LaunchySmarts.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUseShGetFileInfo::CUseShGetFileInfo()
{
	iconlist = GetSystemImageListHandle(false);
}

CUseShGetFileInfo::~CUseShGetFileInfo()
{

}


HIMAGELIST  CUseShGetFileInfo::GetSystemImageListHandle( BOOL bSmallIcon )
{
	HIMAGELIST  hSystemImageList; 
	SHFILEINFO    ssfi; 

	CString winpath;
	LaunchySmarts::GetShellDir(CSIDL_WINDOWS, winpath);
	CString windrive = winpath.Left(3);
   if (bSmallIcon)
   {
	hSystemImageList = 
    (HIMAGELIST)SHGetFileInfo( 
	windrive.GetBuffer(), 
     0, 
     &ssfi, 
     sizeof(SHFILEINFO), 
     SHGFI_SYSICONINDEX | SHGFI_SMALLICON); 
   }
   else
   {
		hSystemImageList = 
    (HIMAGELIST)SHGetFileInfo( 
    windrive.GetBuffer(), 
     0, 
     &ssfi, 
     sizeof(SHFILEINFO), 
     SHGFI_SYSICONINDEX | SHGFI_LARGEICON); 
   }
   return hSystemImageList;
}


int CUseShGetFileInfo::GetFileIconIndex( CString strFileName , BOOL bSmallIcon )
{
	SHFILEINFO    sfi;
	
	if (bSmallIcon)
	{
        SHGetFileInfo(
           (LPCTSTR)strFileName, 
           FILE_ATTRIBUTE_NORMAL,
           &sfi, 
           sizeof(SHFILEINFO), 
           SHGFI_SYSICONINDEX | SHGFI_SMALLICON | SHGFI_USEFILEATTRIBUTES);
	}
	else
	{
		SHGetFileInfo(
           (LPCTSTR)strFileName, 
           FILE_ATTRIBUTE_NORMAL,
           &sfi, 
           sizeof(SHFILEINFO), 
           SHGFI_SYSICONINDEX | SHGFI_LARGEICON | SHGFI_USEFILEATTRIBUTES);
	}

    return sfi.iIcon;
	
}

int CUseShGetFileInfo::GetDirIconIndex(BOOL bSmallIcon )
{
	SHFILEINFO    sfi;
	if (bSmallIcon)
	{
		 SHGetFileInfo(
		 (LPCTSTR)"Doesn't matter", 
		 FILE_ATTRIBUTE_DIRECTORY,
		 &sfi, 
		 sizeof(SHFILEINFO), 
		 SHGFI_SYSICONINDEX | SHGFI_SMALLICON | SHGFI_USEFILEATTRIBUTES);
	}
	else
	{
		 SHGetFileInfo(
		 (LPCTSTR)"Doesn't matter", 
		 FILE_ATTRIBUTE_DIRECTORY,
		 &sfi, 
		 sizeof(SHFILEINFO), 
		 SHGFI_SYSICONINDEX | SHGFI_LARGEICON | SHGFI_USEFILEATTRIBUTES);

	}
    return sfi.iIcon;

}
HICON CUseShGetFileInfo::GetFileIconHandle(CString strFileName, BOOL bSmallIcon)
{
	
	SHFILEINFO    sfi;
	if (bSmallIcon)
	{
		SHGetFileInfo(
		   (LPCTSTR)strFileName, 
		   FILE_ATTRIBUTE_NORMAL,
		   &sfi, 
		   sizeof(SHFILEINFO), 
		   SHGFI_ICON | SHGFI_SMALLICON | SHGFI_USEFILEATTRIBUTES);
	}
	else
	{
		SHGetFileInfo(
		   (LPCTSTR)strFileName, 
		   FILE_ATTRIBUTE_NORMAL,
		   &sfi, 
		   sizeof(SHFILEINFO), 
		   SHGFI_ICON | SHGFI_LARGEICON | SHGFI_USEFILEATTRIBUTES);
	}
	return sfi.hIcon;
}

HICON CUseShGetFileInfo::GetIconHandleNoOverlay(CString strFileName, BOOL bSmallIcon)
{
	int index = GetFileIconIndex(strFileName, bSmallIcon);

	 HICON nH = ImageList_GetIcon(      
			iconlist,
			index,
			ILD_TRANSPARENT 
		);
	 return nH;
}

HICON CUseShGetFileInfo::GetFolderIconHandle(BOOL bSmallIcon )
{
	SHFILEINFO    sfi;
	if (bSmallIcon)
	{
		 SHGetFileInfo(
		 (LPCTSTR)"Doesn't matter", 
		 FILE_ATTRIBUTE_DIRECTORY,
		 &sfi, 
		 sizeof(SHFILEINFO), 
		 SHGFI_ICON | SHGFI_SMALLICON | SHGFI_USEFILEATTRIBUTES);
	}
	else
	{
		 SHGetFileInfo(
		 (LPCTSTR)"Does not matter", 
		 FILE_ATTRIBUTE_DIRECTORY,
		 &sfi, 
		 sizeof(SHFILEINFO), 
		 SHGFI_ICON | SHGFI_LARGEICON | SHGFI_USEFILEATTRIBUTES);
	}
        return sfi.hIcon;
}

CString CUseShGetFileInfo::GetFileType(CString strFileName)
{
	SHFILEINFO    sfi;
	
	SHGetFileInfo(
     (LPCTSTR)strFileName, 
     FILE_ATTRIBUTE_NORMAL,
     &sfi, 
     sizeof(SHFILEINFO), 
     	SHGFI_TYPENAME | SHGFI_USEFILEATTRIBUTES);
     
	return sfi.szTypeName;

}







