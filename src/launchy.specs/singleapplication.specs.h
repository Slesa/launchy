#ifndef SINGLEAPPLICATION_SPECS_H
#define SINGLEAPPLICATION_SPECS_H

#include <QObject>

class SingleApplicationSpecs : public QObject
{
    Q_OBJECT
private slots:
    void runs_as_singleton();
    void reports_not_running_per_default();
    void reports_running_when_started();
    void does_not_start_twice();
};

#endif
