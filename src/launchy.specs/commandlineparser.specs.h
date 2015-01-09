#ifndef COMMANDLINEPARSER_SPECS_H
#define COMMANDLINEPARSER_SPECS_H

#include <QObject>

class CommandLineParserSpecs : public QObject
{
    Q_OBJECT

private slots:
    void option_is_false_per_default();

    void rescue_is_set_with_long_arg();
    void rescue_is_set_with_short_arg();
    void rescan_is_set_with_long_arg();
    void rescan_is_set_with_short_arg();
    void show_is_set_with_long_arg();
    void show_is_set_with_short_arg();
    void options_is_set_with_long_arg();
    void options_is_set_with_short_arg();
    void multiple_is_set_with_long_arg();
    void multiple_is_set_with_short_arg();
    void exit_is_set_with_long_arg();
    void exit_is_set_with_short_arg();
    void log_is_set_with_long_arg();
    void log_is_set_with_short_arg();

    void profile_is_set_with_long_arg();
    void profile_is_set_with_short_arg();
};

#endif
