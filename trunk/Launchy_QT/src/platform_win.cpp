#include "platform_win.h"
#include "globals.h"
#include "main.h"
#include <QKeyEvent>
#include <QDir>

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
	HRESULT hRet = pfnSHGetFolderPath( NULL, iType, NULL, 0, tmp );
	szPath = QString::fromUtf16((const ushort*)tmp);
	FreeLibrary( hInst ); // <-- and here
	return TRUE;
	return 0;
}

QIcon WinIconProvider::icon(const QFileInfo& info) const {
	HICON hico = GetIconHandleNoOverlay(QDir::toNativeSeparators(info.filePath()), false);
	QPixmap qpix = convertHIconToPixmap(hico);
	DeleteObject(hico);
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

	//create image
	HBITMAP winBitmap = CreateBitmap(iconinfo.xHotspot * 2, iconinfo.yHotspot * 2, 1, 32, 0);
	HGDIOBJ oldhdc = SelectObject(hdc, winBitmap);
	DrawIconEx( hdc, 0, 0, icon, iconinfo.xHotspot * 2, iconinfo.yHotspot * 2, 0, 0, DI_NORMAL);
	QPixmap::HBitmapFormat alphaType = QPixmap::PremultipliedAlpha;

	BITMAP bitmapData;
	GetObject(iconinfo.hbmColor, sizeof(BITMAP), &bitmapData);

	QPixmap iconpixmap = QPixmap::fromWinHBITMAP(winBitmap, alphaType);
	QImage img = iconpixmap.toImage();

	if ( bitmapData.bmBitsPixel == 32 ) { //only check 32 bit images for alpha
		for (int y = 0 ; y < iconpixmap.height() && !foundAlpha ; y++) {
			QRgb *scanLine= reinterpret_cast<QRgb *>(img.scanLine(y));
			for (int x = 0; x < img.width() ; x++) {
				if (qAlpha(scanLine[x]) != 0) {
					foundAlpha = true;
					break;
				}
			}
		}
	}

	if (!foundAlpha) {
		//If no alpha was found, we use the mask to set alpha values
		HBITMAP winMask = CreateBitmap(iconinfo.xHotspot * 2, iconinfo.yHotspot * 2, 1, 32, 0);
		SelectObject(hdc, winMask);
		DrawIconEx( hdc, 0, 0, icon, iconinfo.xHotspot * 2, iconinfo.yHotspot * 2, 0, 0, DI_MASK);

		QPixmap maskPixmap = QPixmap::fromWinHBITMAP(winMask, alphaType);
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
		DeleteObject(winMask);
	}

	//dispose resources created by iconinfo call
	DeleteObject(iconinfo.hbmMask);
	DeleteObject(iconinfo.hbmColor);

	SelectObject(hdc, oldhdc); //restore state
	DeleteDC(hdc);
	DeleteObject(winBitmap);
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


	POINT pt={0, 0};
	SIZE sz = {image.width(), image.height()};
	POINT ptSource = {0};
	BLENDFUNCTION bf = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };

	BOOL bRet= UpdateLayeredWindow(winId(), NULL, NULL, &sz, hDC,
		&ptSource, 0, &bf, ULW_ALPHA);

	DeleteObject(bmp);
	DeleteDC(hDC);
}
void QLaunchyAlphaBorder::mouseMoveEvent(QMouseEvent *e)
{
	QPoint p = e->globalPos();
	p -= moveStartPoint;
	move(p);
	((MyWidget*)parentWidget())->MoveFromAlpha(p);
}

void QLaunchyAlphaBorder::contextMenuEvent(QContextMenuEvent *event) {
	((MyWidget*)parentWidget())->contextMenuEvent(event);
}

void PlatformImp::Execute(QString path, QString args) {
	SHELLEXECUTEINFO ShExecInfo;

	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = NULL;
	ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb = NULL;
	ShExecInfo.lpFile = (LPCTSTR) (path).utf16();
	if (args != "") {
		ShExecInfo.lpParameters = (LPCTSTR) args.utf16();
	} else {
		ShExecInfo.lpParameters = NULL;
	}
	ShExecInfo.lpDirectory = NULL;
	ShExecInfo.nShow = SW_NORMAL;
	ShExecInfo.hInstApp = NULL;

	BOOL ret = ShellExecuteEx(&ShExecInfo);	
}