#include "commandlineparser.specs.h"
#include "../launchy/commandlineparser.h"
#include <QtTest/QtTest>



void CommandLineParserSpecs::option_is_false_per_default()
{
    /*
    CommandLineParser parser;
    bool rescue = parser.doRescue();
    QCOMPARE(rescue, false);
    */
}

void CommandLineParserSpecs::rescue_is_set_with_long_arg()
{
    CommandLineParser parser;
    parser.parse(QStringList() << "-rescue");
    bool rescue = parser.doRescue();
    QCOMPARE(rescue, true);
}

void CommandLineParserSpecs::rescue_is_set_with_short_arg()
{
    CommandLineParser parser;
    parser.parse(QStringList() << "-r");
    bool rescue = parser.doRescue();
    QCOMPARE(rescue, true);
}

void CommandLineParserSpecs::rescan_is_set_with_long_arg()
{
    CommandLineParser parser;
    parser.parse(QStringList() << "-rescan");
    bool rescan = parser.doRescan();
    QCOMPARE(rescan, true);
}

void CommandLineParserSpecs::rescan_is_set_with_short_arg()
{
    CommandLineParser parser;
    parser.parse(QStringList() << "-c");
    bool rescan = parser.doRescan();
    QCOMPARE(rescan, true);
}

void CommandLineParserSpecs::show_is_set_with_long_arg()
{
    CommandLineParser parser;
    parser.parse(QStringList() << "-show");
    bool show = parser.doShow();
    QCOMPARE(show, true);
}

void CommandLineParserSpecs::show_is_set_with_short_arg()
{
    CommandLineParser parser;
    parser.parse(QStringList() << "-s");
    bool show = parser.doShow();
    QCOMPARE(show, true);
}

void CommandLineParserSpecs::options_is_set_with_long_arg()
{
    CommandLineParser parser;
    parser.parse(QStringList() << "-options");
    bool option = parser.doOptions();
    QCOMPARE(option, true);
}

void CommandLineParserSpecs::options_is_set_with_short_arg()
{
    CommandLineParser parser;
    parser.parse(QStringList() << "-o");
    bool option = parser.doOptions();
    QCOMPARE(option, true);
}

void CommandLineParserSpecs::multiple_is_set_with_long_arg()
{
    CommandLineParser parser;
    parser.parse(QStringList() << "-multiple");
    bool multiple = parser.doMultiple();
    QCOMPARE(multiple, true);
}

void CommandLineParserSpecs::multiple_is_set_with_short_arg()
{
    CommandLineParser parser;
    parser.parse(QStringList() << "-m");
    bool multiple = parser.doMultiple();
    QCOMPARE(multiple, true);
}

void CommandLineParserSpecs::exit_is_set_with_long_arg()
{
    CommandLineParser parser;
    parser.parse(QStringList() << "-exit");
    bool exit = parser.doExit();
    QCOMPARE(exit, true);
}

void CommandLineParserSpecs::exit_is_set_with_short_arg()
{
    CommandLineParser parser;
    parser.parse(QStringList() << "-x");
    bool exit = parser.doExit();
    QCOMPARE(exit, true);
}

void CommandLineParserSpecs::log_is_set_with_long_arg()
{
    CommandLineParser parser;
    parser.parse(QStringList() << "-log");
    bool log = parser.doLog();
    QCOMPARE(log, true);
}

void CommandLineParserSpecs::log_is_set_with_short_arg()
{
    CommandLineParser parser;
    parser.parse(QStringList() << "-l");
    bool log = parser.doLog();
    QCOMPARE(log, true);
}

void CommandLineParserSpecs::profile_is_set_with_long_arg()
{
    CommandLineParser parser;
    parser.parse(QStringList() << "--profile=Mine");
    QString profile = parser.getProfile();
    QCOMPARE(profile, QString("Mine"));
}

void CommandLineParserSpecs::profile_is_set_with_short_arg()
{
    CommandLineParser parser;
    parser.parse(QStringList() << "-p=Mine");
    QString profile = parser.getProfile();
    QCOMPARE(profile, QString("Mine"));
}
