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


