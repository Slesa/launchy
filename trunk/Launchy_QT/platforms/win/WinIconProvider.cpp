/*
Launchy: Application Launcher
Copyright (C) 2007-2009  Josh Karlin, Simon Capewell

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


#include "precompiled.h"
#include "platform_win_util.h"
#include "WinIconProvider.h"


HRESULT (WINAPI* fnSHCreateItemFromParsingName)(PCWSTR, IBindCtx *, REFIID, void **) = NULL;


WinIconProvider::WinIconProvider() :
	preferredSize(32)
{
	// Load Vista/7 specific API pointers
	HMODULE hLib = GetModuleHandleW(L"shell32");
	if (hLib)
	{
		(FARPROC&)fnSHCreateItemFromParsingName = GetProcAddress(hLib, "SHCreateItemFromParsingName");
	}
}


WinIconProvider::~WinIconProvider()
{
}


void WinIconProvider::setPreferredIconSize(int size)
{
	preferredSize = size;
}


QIcon WinIconProvider::icon(const QFileInfo& info) const
{
	QIcon retIcon;
	QString fileExtension = info.suffix().toLower();

	if (fileExtension == "png" ||
		fileExtension == "bmp" ||
		fileExtension == "jpg" ||
		fileExtension == "jpeg")
	{
		retIcon = QIcon(info.filePath());
	}
	else if (fileExtension == "cpl")
	{
		HICON hIcon;
		QString filePath = QDir::toNativeSeparators(info.filePath());
		ExtractIconEx(filePath.utf16(), 0, &hIcon, NULL, 1);
        retIcon = QIcon(convertHIconToPixmap(hIcon));
		DestroyIcon(hIcon);
	}
	else
	{
		QString filePath = QDir::toNativeSeparators(info.filePath());

		// Get the icon index using SHGetFileInfo
		SHFILEINFO sfi = {0};

		// To avoid network hangs, explicitly fetch the My Computer icon for UNCs
		QRegExp re("\\\\\\\\([a-z]+\\\\?)?$", Qt::CaseInsensitive);
		SHGetFileInfo(re.exactMatch(filePath) ? L"::{20D04FE0-3AEA-1069-A2D8-08002B30309D}" : filePath.utf16(),
			0, &sfi, sizeof(sfi), SHGFI_SYSICONINDEX);

		// An icon index of 3 is the generic file icon
		if (sfi.iIcon > 0 && sfi.iIcon != 3)
		{
			// Retrieve the system image list.
			// To get the 48x48 icons, use SHIL_EXTRALARGE
			// To get the 256x256 icons (Vista only), use SHIL_JUMBO
			int imageListIndex;
			if (preferredSize <= 16)
				imageListIndex = SHIL_SMALL;
			else if (preferredSize <= 32)
				imageListIndex = SHIL_LARGE;
			else
				imageListIndex = SHIL_EXTRALARGE;

			if (!addIconFromShellFactory(filePath, retIcon))
				addIconFromImageList(imageListIndex, sfi.iIcon, retIcon);

			// Ensure there's also a 32x32 icon
			if (imageListIndex == SHIL_EXTRALARGE)
				addIconFromImageList(SHIL_LARGE, sfi.iIcon, retIcon);
		}
		else if (info.isSymLink() || fileExtension == "lnk") // isSymLink is case sensitive when it perhaps shouldn't be
		{
			QFileInfo targetInfo(info.symLinkTarget());
			retIcon = icon(targetInfo);
		}
		else
		{
			retIcon = QFileIconProvider::icon(info);
		}		
	}

	return retIcon;
}


bool WinIconProvider::addIconFromImageList(int imageListIndex, int iconIndex, QIcon& icon) const
{
	HICON hIcon = 0;
	IImageList* imageList;
	HRESULT hResult = SHGetImageList(imageListIndex, IID_IImageList, (void**)&imageList);
	if (hResult == S_OK)
	{
		hResult = ((IImageList*)imageList)->GetIcon(iconIndex, ILD_TRANSPARENT, &hIcon);
	}
	if (hResult == S_OK && hIcon)
	{
		icon.addPixmap(convertHIconToPixmap(hIcon));
		DestroyIcon(hIcon);
	}

	return SUCCEEDED(hResult);
}


// On Vista or 7 we could use SHIL_JUMBO to get a 256x256 icon,
// but we'll use SHCreateItemFromParsingName for its built in image scaling
bool WinIconProvider::addIconFromShellFactory(QString filePath, QIcon& icon) const
{
	HRESULT hr = S_FALSE;

	if (fnSHCreateItemFromParsingName)
	{
		IShellItem* psi = NULL;
		hr = fnSHCreateItemFromParsingName(filePath.utf16(), 0, IID_IShellItem, (void**)&psi);
		if (hr == S_OK)
		{
			IShellItemImageFactory* psiif = NULL;
			hr = psi->QueryInterface(IID_IShellItemImageFactory, (void**)&psiif);
			if (hr == S_OK)
			{
				HBITMAP iconBitmap = NULL;
				SIZE iconSize = {preferredSize, preferredSize};
				hr = psiif->GetImage(iconSize, SIIGBF_RESIZETOFIT | SIIGBF_ICONONLY, &iconBitmap);
				if (hr == S_OK)
				{
					QPixmap iconPixmap = QPixmap::fromWinHBITMAP(iconBitmap, QPixmap::PremultipliedAlpha);
					icon.addPixmap(iconPixmap);
					DeleteObject(iconBitmap);
				}

				psiif->Release();
			}
			psi->Release();
		}
	}

	return SUCCEEDED(hr);
}


// Slightly modified from QT's private version of the same function.
QPixmap WinIconProvider::convertHIconToPixmap(const HICON icon) const
{
    bool foundAlpha = false;
    HDC screenDevice = GetDC(0);
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
	DeleteDC(screenDevice);
    return QPixmap::fromImage(img);
}
