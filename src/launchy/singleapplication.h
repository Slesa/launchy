#ifndef SINGLEAPPLICATION_H
#define SINGLEAPPLICATION_H

#include <QSharedMemory>

class QString;

class SingleApplication
{
public:
    static SingleApplication* instance();
    ~SingleApplication();
    bool isRunning();
    bool start();
    void release();
private:
    SingleApplication();
    QString applicationKey();
private:
    static SingleApplication* _instance;
    static QSharedMemory _appLock;
};

#endif
