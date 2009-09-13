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
	LimitSingleInstance(TCHAR *strMutexName)
	{
		//Make sure that you use a name that is unique for this application otherwise
		//two apps may think they are the same if they are using same name for
		//3rd parm to CreateMutex
		mutex = CreateMutex(NULL, FALSE, strMutexName); //do early
		lastError = GetLastError(); //save for use later...
	}

	~LimitSingleInstance() 
	{
		if (mutex)  //Do not forget to close handles.
		{
			CloseHandle(mutex); //Do as late as possible.
			mutex = NULL; //Good habit to be in.
		}
	}

	bool IsAnotherInstanceRunning() 
	{
		return (ERROR_ALREADY_EXISTS == lastError);
	}

private:
	HANDLE mutex;
	DWORD  lastError;
};


#endif
