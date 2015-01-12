#include "singleapplication.specs.h"
#include "singleapplication.h"
#include <QtTest/QtTest>

void SingleApplicationSpecs::runs_as_singleton()
{
    SingleApplication* app = SingleApplication::instance();
    SingleApplication* cmp = SingleApplication::instance();
    QCOMPARE(app, cmp);
    app->release();
    //delete app;
}

void SingleApplicationSpecs::reports_not_running_per_default()
{
    SingleApplication* app = SingleApplication::instance();
    bool running = app->isRunning();
    QCOMPARE(running, false);
    app->release();
    //delete app;
}

void SingleApplicationSpecs::reports_running_when_started()
{
    SingleApplication* app = SingleApplication::instance();
    QCOMPARE(app->start(), true);
    QCOMPARE(app->isRunning(), true);
    app->release();
    //delete app;
}

void SingleApplicationSpecs::does_not_start_twice()
{
    SingleApplication* app = SingleApplication::instance();
    QCOMPARE(app->start(), true);
    QCOMPARE(app->start(), false);
    delete app;
    //app->release();
}
