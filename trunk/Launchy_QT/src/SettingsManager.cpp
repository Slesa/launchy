/*
Launchy: Application Launcher
Copyright (C) 2010  Josh Karlin, Simon Capewell

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


#include "precompiled.h"
#include "SettingsManager.h"
#include "main.h"
#include "globals.h"


SettingsManager settings;

const char* iniName = "/launchy.ini";
const char* dbName = "/launchy.db";
const char* historyName = "/history.db";


SettingsManager::SettingsManager()
{
	portable = false;
}


void SettingsManager::load()
{
	if (gSettings != NULL)
	{
		delete gSettings;
		gSettings = NULL;
	}

	// Load settings
	dirs = platform->getDirectories();
	portable = QFile::exists(dirs["portableConfig"][0] + iniName);
	gSettings = new QSettings(configDirectory(portable) + iniName, QSettings::IniFormat);

	qDebug() << QString("Loading settings in %1 mode from %2").arg(portable ? "portable" : "installed", iniName);
}


bool SettingsManager::isPortable() const
{
	return portable;
}


QList<QString> SettingsManager::directory(QString name) const
{
	return dirs[name];
}


QString SettingsManager::catalogFilename() const
{
	return configDirectory(portable) + dbName;
}


QString SettingsManager::historyFilename() const
{
	return configDirectory(portable) + historyName;
}


// Find the skin with the specified name ensuring that it contains at least a stylesheet
QString SettingsManager::skinPath(const QString& skinName) const
{
	QString directory;

    foreach(QString dir, dirs["skins"])
	{
		dir += QString("/") + skinName + "/";
        if (QFile::exists(dir + "style.qss"))
		{
			directory = dir;
			break;
		}
	}

    return directory;
}


// Switch between portable and installed mode
void SettingsManager::setPortable(bool makePortable)
{
	if (makePortable != portable)
	{
		qDebug() << QString("Converting to %1 mode").arg(makePortable ? "portable" : "installed");

		// Destroy the QSettings object first so it writes any changes to disk
		if (gSettings != NULL)
		{
			delete gSettings;
			gSettings = NULL;
		}

		QString oldDir = configDirectory(portable);
		QString oldIniName = oldDir + iniName;
		QString oldDbName = oldDir + dbName;
		QString oldHistoryName = oldDir + historyName;

		// Copy the settings to the new location and delete old settings
		QString newDir = configDirectory(makePortable);
		QFile::copy(oldIniName, newDir + iniName);
		QFile::copy(oldDbName, newDir + dbName);
		QFile::copy(oldHistoryName, newDir + historyName);

		QFile::remove(oldIniName);
		QFile::remove(oldDbName);
		QFile::remove(oldHistoryName);

		load();
	}
}


// Delete all settings files in both installed and portable directories
void SettingsManager::removeAll()
{
	QFile::remove(configDirectory(false) + iniName);
	QFile::remove(configDirectory(false) + dbName);
	QFile::remove(configDirectory(false) + historyName);

	QFile::remove(configDirectory(true) + iniName);
	QFile::remove(configDirectory(true) + dbName);
	QFile::remove(configDirectory(true) + historyName);
}


// Get the configuration directory
QString SettingsManager::configDirectory(bool portable) const
{
	return dirs[portable ? "portableConfig" : "config"][0];
}
