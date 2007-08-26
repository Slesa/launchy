#ifndef GLOBALS_H
#define GLOBALS_H

#include <QWidget>
#include <QSettings>
#include "catalog_builder.h"

#define LAUNCHY_VERSION 200
#define LAUNCHY_VERSION_STRING "2.0"

#define HASH_LAUNCHY 0
#define LABEL_FILE 0

extern QWidget* gMainWidget;
extern QSettings* gSettings;
extern CatBuilder* gBuilder;


extern QString gSearchTxt;




#endif
