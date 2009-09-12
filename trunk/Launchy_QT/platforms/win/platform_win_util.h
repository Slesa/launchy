#ifndef __PLATFORM_WIN_UTIL_H
#define __PLATFORM_WIN_UTIL_H


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
#include <QWidget>


#pragma warning (disable : 4089)

void UpdateEnvironment();
QString GetShellDirectory(int type);


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


#endif
