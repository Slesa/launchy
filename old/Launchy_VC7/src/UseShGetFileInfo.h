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

// This file was created on February 5th 2003. By Doru Cioata
//
//


#if !defined(AFX_USESHGETFILEINFO_H__CE3A83A6_10C3_46BC_9D55_EFB612864292__INCLUDED_)
#define AFX_USESHGETFILEINFO_H__CE3A83A6_10C3_46BC_9D55_EFB612864292__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning (disable : 4089)

class CUseShGetFileInfo{

public:
	CUseShGetFileInfo();
	virtual ~CUseShGetFileInfo();
	
	//get the system's image list
	HIMAGELIST  GetSystemImageListHandle( BOOL bSmallIcon );
	
	//get the image's index in the system's image list
	int GetFileIconIndex( CString strFileName , BOOL bSmallIcon);
	int GetDirIconIndex(BOOL bSmallIcon);

	//get a handle to the icon
	HICON GetFileIconHandle(CString strFileName,BOOL bSmallIcon);
	HICON GetIconHandleNoOverlay(CString strFileName,BOOL bSmallIcon);
	HICON GetFolderIconHandle(BOOL bSmallIcon );

	//get file type
	CString GetFileType(CString strFileName);

	HIMAGELIST iconlist;
};

#endif 


