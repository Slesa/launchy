#ifndef PLATFORM_WIN
#define PLATFORM_WIN



#define VC_EXTRALEAN
#define WINVER 0x05100
#define _WIN32_WINNT 0x0510	
#define _WIN32_WINDOWS 0x0510 
#define _WIN32_IE 0x0600


#ifndef _UNICODE
#define _UNICODE
#endif



#include <QFileIconProvider>
#include <windows.h>
#include <shlobj.h>
#include <TCHAR.h>
#include <QT>
#include <QIcon>
#include <QPixmap>
#include <QString>
#include <QtDebug>
#include <QMouseEvent>
#include "platform_base.h"
#include "globals.h"


#pragma warning (disable : 4089)
BOOL GetShellDir(int iType, QString& szPath);




class WinIconProvider : QFileIconProvider
{
public:
	WinIconProvider() {
		iconlist = GetSystemImageListHandle(false);
	}
	~WinIconProvider() {

	}


	virtual QIcon icon(const QFileInfo& info) const;

private:

	HIMAGELIST iconlist;	


private:
	HIMAGELIST GetSystemImageListHandle( bool bSmallIcon );


	int GetFileIconIndex( QString strFileName , BOOL bSmallIcon ) const;

	int GetDirIconIndex(BOOL bSmallIcon );
	HICON GetFileIconHandle(QString strFileName, BOOL bSmallIcon);

	HICON GetIconHandleNoOverlay(QString strFileName, BOOL bSmallIcon) const;

	HICON GetFolderIconHandle(BOOL bSmallIcon );

	QString GetFileType(QString strFileName);

	QPixmap convertHIconToPixmap( const HICON icon) const;

};


class QLaunchyAlphaBorder : public QWidget {
	Q_OBJECT  
private:
	QPoint moveStartPoint;
public:
	QLaunchyAlphaBorder(QWidget *parent)
	: QWidget(parent,Qt::Tool | Qt::FramelessWindowHint) {
	}
	~QLaunchyAlphaBorder() {}
	void SetImage(QString name);
	void RepositionWindow(QPoint pos) {
		move(pos);
	}
	void contextMenuEvent(QContextMenuEvent *event);
	void QLaunchyAlphaBorder::mousePressEvent(QMouseEvent *e)
	{
		moveStartPoint = e->pos();
	}

	void QLaunchyAlphaBorder::mouseMoveEvent(QMouseEvent *e);

};



class PlatformImp : public PlatformBase {
private:
	QLaunchyAlphaBorder* alpha;
	HWND hotkeyWnd;
	HANDLE m1, mg1;
	HDC hDC;
public:
	PlatformImp() : PlatformBase() 		
	{
		alpha = NULL; 
		icons = (QFileIconProvider*) new WinIconProvider();

		// Create application mutexes so that installer knows when
		// Launchy is running
		m1 = CreateMutex(NULL,0,_T("LaunchyMutex"));
		mg1 = CreateMutex(NULL,0,_T("Global\\LaunchyMutex"));
	}
	~PlatformImp() {
		if (icons != NULL)
			delete icons;
		CloseHandle(m1);
		CloseHandle(mg1);

	}
	// Mandatory functions
	QString GetSettingsDirectory() { 
		QString ret;	
		GetShellDir(CSIDL_APPDATA, ret);
		return ret;
	};


	QList<Directory> GetInitialDirs() {
		QList<Directory> list;
		Directory tmp;
		QString ret;
		GetShellDir(CSIDL_COMMON_STARTMENU, ret);
		tmp.name = ret;
		tmp.types << "*.lnk";
		list.append(tmp);
		GetShellDir(CSIDL_STARTMENU, ret);		
		tmp.name = ret;
		list.append(tmp);
		tmp.name = "Utilities/";
		tmp.indexDirs = true;
		list.append(tmp);
		return list;
	}


	void Execute(QString path, QString args);
	void AddToNotificationArea() {};
	void RemoveFromNotificationArea() {};

	// Alpha border functions	
	bool CreateAlphaBorder(QWidget* w, QString ImageName) { 
		if (alpha != NULL) {
			// Save the position
			RECT r;
			GetWindowRect(alpha->winId(), &r);
			DestroyAlphaBorder();
			alpha = new QLaunchyAlphaBorder(w);
			alpha->SetImage(ImageName);
			QRect rect(0,0,500,100);
			alpha->setGeometry(rect);
			alpha->show();

		} else {
			alpha = new QLaunchyAlphaBorder(w);
			alpha->SetImage(ImageName);
		}
		return true;
	}
	void DestroyAlphaBorder() { delete alpha; alpha = NULL; return;}
	void MoveAlphaBorder(QPoint pos) { if (alpha != NULL) alpha->RepositionWindow(pos); }
	void HideAlphaBorder() { if (alpha != NULL) alpha->hide(); }
	void ShowAlphaBorder() { if (alpha != NULL) alpha->show(); }



};
#endif