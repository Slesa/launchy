
////////////////////////////////////////////////////////////
//                                                        //
// ClassName.cpp: implementation of the CClassName class. //
//                                                        //
////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "UseShGetFileInfo.h"

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

   if (bSmallIcon)
   {
	hSystemImageList = 
    (HIMAGELIST)SHGetFileInfo( 
    (LPCTSTR)_T("c:\\"), 
     0, 
     &ssfi, 
     sizeof(SHFILEINFO), 
     SHGFI_SYSICONINDEX | SHGFI_SMALLICON); 
   }
   else
   {
		hSystemImageList = 
    (HIMAGELIST)SHGetFileInfo( 
    (LPCTSTR)_T("c:\\"), 
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







