/*
Launchy: Application Launcher
Copyright (C) 2007  Josh Karlin

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


#include "platform_win.h"
#include "globals.h"
#include "main.h"
#include <QKeyEvent>
#include <QDir>
#include <QObject>
#include <QtGui>


BOOL GetShellDir(int iType, QString& szPath)
{
	HINSTANCE hInst = ::LoadLibrary( _T("shell32.dll") );
	if ( NULL == hInst )
	{
		return FALSE;
	}

	HRESULT (__stdcall *pfnSHGetFolderPath)( HWND, int, HANDLE, DWORD, LPWSTR );


	pfnSHGetFolderPath = reinterpret_cast<HRESULT (__stdcall *)( HWND, int, HANDLE, DWORD, LPWSTR )>( GetProcAddress( hInst,"SHGetFolderPathW" ) );

	if ( NULL == pfnSHGetFolderPath )
	{
		FreeLibrary( hInst ); // <-- here
		return FALSE;
	}

	TCHAR tmp[_MAX_PATH];
	pfnSHGetFolderPath( NULL, iType, NULL, 0, tmp );
	szPath = QString::fromUtf16((const ushort*)tmp);
	FreeLibrary( hInst ); // <-- and here
	return TRUE;
	return 0;
}

QIcon WinIconProvider::icon(const QFileInfo& info) const {
	HICON hico = GetIconHandleNoOverlay(QDir::toNativeSeparators(info.filePath()), false);
	QPixmap qpix = convertHIconToPixmap(hico);
//	qpix.fromWinHBITMAP(hico, QPixmap::PremultipliedAlpha);
	DestroyIcon(hico);
	return qpix;
}

HIMAGELIST WinIconProvider::GetSystemImageListHandle( bool bSmallIcon ) 
{
	HIMAGELIST  hSystemImageList; 
	SHFILEINFO    ssfi; 

	QString winpath;
	GetShellDir(CSIDL_WINDOWS, winpath);
	QString windrive = QDir::toNativeSeparators(winpath.mid(0,3));
	if (bSmallIcon)
	{
		hSystemImageList = 
			(HIMAGELIST)SHGetFileInfo( 
			(LPCTSTR) windrive.utf16(), 
			0, 
			&ssfi, 
			sizeof(SHFILEINFO), 
			SHGFI_SYSICONINDEX | SHGFI_SMALLICON); 
	}
	else
	{
		hSystemImageList = 
			(HIMAGELIST)SHGetFileInfo( 
			(LPCTSTR) windrive.utf16(), 
			0, 
			&ssfi, 
			sizeof(SHFILEINFO), 
			SHGFI_SYSICONINDEX | SHGFI_LARGEICON); 
	}
	return hSystemImageList;
}

int WinIconProvider::GetFileIconIndex( QString strFileName , BOOL bSmallIcon ) const
{
	DWORD_PTR ret;
	SHFILEINFO    sfi;

	if (bSmallIcon)
	{
		ret = SHGetFileInfo(
			(LPCTSTR) strFileName.utf16(), 
			FILE_ATTRIBUTE_NORMAL,
			&sfi, 
			sizeof(SHFILEINFO), 
			SHGFI_SYSICONINDEX | SHGFI_SMALLICON | SHGFI_USEFILEATTRIBUTES);
	}
	else
	{
		ret = SHGetFileInfo(
			(LPCTSTR) strFileName.utf16(), 
			FILE_ATTRIBUTE_NORMAL,
			&sfi, 
			sizeof(SHFILEINFO), 
			SHGFI_SYSICONINDEX | SHGFI_LARGEICON | SHGFI_USEFILEATTRIBUTES);
	}
	// The following removes the unknown icon
	//	if (sfi.iIcon == 3)
	//		sfi.iIcon = -1;

	return sfi.iIcon;
}

int WinIconProvider::GetDirIconIndex(BOOL bSmallIcon )
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
HICON WinIconProvider::GetFileIconHandle(QString strFileName, BOOL bSmallIcon)
{

	SHFILEINFO    sfi;
	if (bSmallIcon)
	{
		SHGetFileInfo(
			(LPCTSTR) strFileName.utf16(), 
			FILE_ATTRIBUTE_NORMAL,
			&sfi, 
			sizeof(SHFILEINFO), 
			SHGFI_ICON | SHGFI_SMALLICON | SHGFI_USEFILEATTRIBUTES);
	}
	else
	{
		SHGetFileInfo(
			(LPCTSTR) strFileName.utf16(), 
			FILE_ATTRIBUTE_NORMAL,
			&sfi, 
			sizeof(SHFILEINFO), 
			SHGFI_ICON | SHGFI_LARGEICON | SHGFI_USEFILEATTRIBUTES);
	}
	return sfi.hIcon;
}

HICON WinIconProvider::GetIconHandleNoOverlay(QString strFileName, BOOL bSmallIcon) const
{
	int index = GetFileIconIndex(strFileName, bSmallIcon);

	HICON nH = ImageList_GetIcon(      
		iconlist,
		index,
		ILD_TRANSPARENT 
		);
	return nH;
}

HICON WinIconProvider::GetFolderIconHandle(BOOL bSmallIcon )
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

QString WinIconProvider::GetFileType(QString strFileName)
{
	SHFILEINFO    sfi;

	SHGetFileInfo(
		(LPCTSTR) strFileName.utf16(), 
		FILE_ATTRIBUTE_NORMAL,
		&sfi, 
		sizeof(SHFILEINFO), 
		SHGFI_TYPENAME | SHGFI_USEFILEATTRIBUTES);

	QString ret = QString::fromUtf16((const ushort*) sfi.szTypeName);
	return ret;
}
QPixmap WinIconProvider::convertHIconToPixmap( const HICON icon) const
{
    bool foundAlpha = false;
    HDC screenDevice = qt_win_display_dc();
    HDC hdc = CreateCompatibleDC(screenDevice);

    ICONINFO iconinfo;
    GetIconInfo(icon, &iconinfo); //x and y Hotspot describes the icon center

    BITMAPINFOHEADER bitmapInfo;
    bitmapInfo.biSize        = sizeof(BITMAPINFOHEADER);
    bitmapInfo.biWidth       = iconinfo.xHotspot * 2;
    bitmapInfo.biHeight      = iconinfo.yHotspot * 2;
    bitmapInfo.biPlanes      = 1;
    bitmapInfo.biBitCount    = 32;
    bitmapInfo.biCompression = BI_RGB;
    bitmapInfo.biSizeImage   = 0;
    bitmapInfo.biXPelsPerMeter = 0;
    bitmapInfo.biYPelsPerMeter = 0;
    bitmapInfo.biClrUsed       = 0;
    bitmapInfo.biClrImportant  = 0;
    DWORD* bits;

    HBITMAP winBitmap = CreateDIBSection(hdc, (BITMAPINFO*)&bitmapInfo, DIB_RGB_COLORS, (VOID**)&bits, NULL, 0);
    HGDIOBJ oldhdc = (HBITMAP)SelectObject(hdc, winBitmap);
    DrawIconEx( hdc, 0, 0, icon, iconinfo.xHotspot * 2, iconinfo.yHotspot * 2, 0, 0, DI_NORMAL);

    QPixmap::HBitmapFormat alphaType = QPixmap::PremultipliedAlpha;
    QPixmap iconpixmap = QPixmap::fromWinHBITMAP(winBitmap, alphaType);
    QImage img = iconpixmap.toImage();

    for (int y = 0 ; y < iconpixmap.height() && !foundAlpha ; y++) {
        QRgb *scanLine= reinterpret_cast<QRgb *>(img.scanLine(y));
        for (int x = 0; x < img.width() ; x++) {
            if (qAlpha(scanLine[x]) != 0) {
                foundAlpha = true;
                break;
            }
        }
    }

    if (!foundAlpha) {
        //If no alpha was found, we use the mask to set alpha values
        DrawIconEx( hdc, 0, 0, icon, iconinfo.xHotspot * 2, iconinfo.yHotspot * 2, 0, 0, DI_MASK);
        QPixmap maskPixmap = QPixmap::fromWinHBITMAP(winBitmap, alphaType);
        QImage mask = maskPixmap.toImage();

        for (int y = 0 ; y< iconpixmap.height() ; y++){
            QRgb *scanlineImage = reinterpret_cast<QRgb *>(img.scanLine(y));
            QRgb *scanlineMask = reinterpret_cast<QRgb *>(mask.scanLine(y));
            for (int x = 0; x < img.width() ; x++){
                if (qRed(scanlineMask[x]) != 0)
                    scanlineImage[x] = 0; //mask out this pixel
                else
                    scanlineImage[x] |= 0xff000000; // set the alpha channel to 255
            }
        }
    }

    //dispose resources created by iconinfo call
    DeleteObject(iconinfo.hbmMask);
    DeleteObject(iconinfo.hbmColor);

    SelectObject(hdc, oldhdc); //restore state
    DeleteObject(winBitmap);
    DeleteDC(hdc);
    return QPixmap::fromImage(img);
}




void QLaunchyAlphaBorder::SetImage(QString name)
{
	SetWindowLong(winId(), GWL_EXSTYLE,
		GetWindowLong(winId(), GWL_EXSTYLE) | WS_EX_LAYERED);
	QImage image(name);
	image.convertToFormat(QImage::Format_ARGB32_Premultiplied);


	QPixmap pix = QPixmap::fromImage(image);
	HBITMAP bmp = pix.toWinHBITMAP(QPixmap::PremultipliedAlpha);
	HDC hDC = CreateCompatibleDC(NULL);
	SelectObject(hDC, bmp);


	SIZE sz = {image.width(), image.height()};
	POINT ptSource = {0};
	BLENDFUNCTION bf = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };

	UpdateLayeredWindow(winId(), NULL, NULL, &sz, hDC,
		&ptSource, 0, &bf, ULW_ALPHA);

	DeleteObject(bmp);
	DeleteDC(hDC);
}
void QLaunchyAlphaBorder::SetAlphaOpacity(double trans)
{ 

//	HDC hDC = 
//	if (alpha != NULL) alpha->setWindowOpacity(trans); 
	BLENDFUNCTION bf = { AC_SRC_OVER, 0, (int) (trans * 255.0), AC_SRC_ALPHA };
	UpdateLayeredWindow(winId(), NULL, NULL, NULL, NULL,
		NULL, 0, &bf, ULW_ALPHA);
}





/*
void PlatformImp::Execute(QString path, QString args) {
	SHELLEXECUTEINFO ShExecInfo;

	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = SEE_MASK_FLAG_NO_UI;
	ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb = NULL;
	ShExecInfo.lpFile = (LPCTSTR) (path).utf16();
	if (args != "") {
		ShExecInfo.lpParameters = (LPCTSTR) args.utf16();
	} else {
		ShExecInfo.lpParameters = NULL;
	}
	QDir dir(path);
	dir.cdUp();
	ShExecInfo.lpDirectory = (LPCTSTR)QDir::toNativeSeparators(dir.absolutePath()).utf16();
	ShExecInfo.nShow = SW_NORMAL;
	ShExecInfo.hInstApp = NULL;

	BOOL ret = ShellExecuteEx(&ShExecInfo);	
}
*/
