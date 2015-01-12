#include "singleapplication.h"
#include <QCoreApplication>
#include <QString>
#include <QMutex>
#include <QDebug>

SingleApplication* SingleApplication::_instance = NULL;
QSharedMemory SingleApplication::_appLock;

SingleApplication* SingleApplication::instance()
{
    static QMutex mutex;
    if(_instance==NULL)
    {
        mutex.lock();
        _instance = new SingleApplication();
        mutex.unlock();
    }
    return _instance;
}

bool SingleApplication::isRunning()
{
    QSharedMemory sharedMemory;
    sharedMemory.setKey(applicationKey());
    if(sharedMemory.attach())
    {
        sharedMemory.detach();
        return true;
    }
    return false;
}

bool SingleApplication::start()
{
    if(!_appLock.create(1))
    {
        return false;
    }
    return true;
}

void SingleApplication::release()
{
    static QMutex mutex;
    mutex.lock();
    if(_instance!=NULL)
    {
        _instance->_appLock.detach();
        delete _instance;
    }
    _instance = NULL;
    mutex.unlock();
}

SingleApplication::SingleApplication()
{
    _appLock.setKey(applicationKey());
}

SingleApplication::~SingleApplication()
{
//    release();
}

QString SingleApplication::applicationKey()
{
    return qApp->applicationName() + "@applock";
}
