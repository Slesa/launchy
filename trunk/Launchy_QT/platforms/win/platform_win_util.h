#ifndef PLATFORM_WIN_UTIL
#define PLATFORM_WIN_UTIL

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
//#include <stdlib.h>
//#include <stdio.h>
#include <TCHAR.h>
#include <QT>
#include <QIcon>
#include <QPixmap>
#include <QString>
#include <QtDebug>
#include <QMouseEvent>
#include <QWidget>



#pragma warning (disable : 4089)
BOOL GetShellDir(int iType, QString& szPath);


class LimitSingleInstance
{
public:
  DWORD  m_dwLastError;
  HANDLE m_hMutex;

public:
  LimitSingleInstance(TCHAR *strMutexName)
  {
    //Make sure that you use a name that is unique for this application otherwise
    //two apps may think they are the same if they are using same name for
    //3rd parm to CreateMutex
    m_hMutex = CreateMutex(NULL, FALSE, strMutexName); //do early
    m_dwLastError = GetLastError(); //save for use later...
  }
   
  ~LimitSingleInstance() 
  {
    if (m_hMutex)  //Do not forget to close handles.
    {
       CloseHandle(m_hMutex); //Do as late as possible.
       m_hMutex = NULL; //Good habit to be in.
    }
  }

  BOOL IsAnotherInstanceRunning() 
  {
    return (ERROR_ALREADY_EXISTS == m_dwLastError);
  }
};

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

	void SetAlphaOpacity(double trans);
	void contextMenuEvent(QContextMenuEvent* evt) {
	    emit menuEvent(evt);
	}
	
	void mousePressEvent(QMouseEvent *event) {
	    emit mousePress(event);
	}
	void mouseMoveEvent(QMouseEvent *e) {
	    emit mouseMove(e);
	}
 signals:
	void menuEvent(QContextMenuEvent*);
	void mousePress(QMouseEvent*);
	void mouseMove(QMouseEvent*);
};

#endif
