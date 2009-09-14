#ifndef __WINICONPROVIDER_H
#define __WINICONPROVIDER_H


#include <windows.h>
#include <shlobj.h>
#include <QFileIconProvider>


class WinIconProvider : QFileIconProvider
{
public:
	WinIconProvider();
	~WinIconProvider();

	virtual QIcon icon(const QFileInfo& info) const;

private:
	HIMAGELIST GetSystemImageListHandle( bool bSmallIcon );
	int GetFileIconIndex( QString strFileName , BOOL bSmallIcon ) const;
	int GetDirIconIndex(BOOL bSmallIcon );
	HICON GetFileIconHandle(QString strFileName, BOOL bSmallIcon);
	HICON GetIconHandleNoOverlay(QString strFileName, BOOL bSmallIcon) const;
	HICON GetFolderIconHandle(BOOL bSmallIcon );
	QString GetFileType(QString strFileName);
	QPixmap convertHIconToPixmap( const HICON icon) const;

	HIMAGELIST iconlist;	
};


#endif
