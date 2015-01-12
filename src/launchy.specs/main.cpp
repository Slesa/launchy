#include <QCoreApplication>
#include <QtTest>
#include "commandlineparser.specs.h"
#include "singleapplication.specs.h"

int main(int argc, char** argv) 
{
    QCoreApplication app(argc, argv);

    bool success = true;

    CommandLineParserSpecs commandLineParserSpecs;
    success |= QTest::qExec(&commandLineParserSpecs, argc, argv);

    SingleApplicationSpecs singleApplicationSpecs;
    success |= QTest::qExec(&singleApplicationSpecs, argc, argv);

    return success;
}
