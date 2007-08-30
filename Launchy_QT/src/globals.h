/*
Launchy: Application Launcher
Copyright (C) 2007  Josh Karlin

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

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
