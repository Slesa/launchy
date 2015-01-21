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
#include "settingsmanager.h"
#include "platform_base.h"
//#include "launchywidget.h"
#include "globals.h"
#include <QMessageBox>
#include <QDateTime>

SettingsManager settings;

const char* SettingsManager::SkinDefault = "Default";

const char* iniName = "/launchy.ini";
const char* dbName = "/launchy.db";
const char* historyName = "/history.db";


SettingsManager::SettingsManager()
    : _settings(NULL)
{
	portable = false;
}


void SettingsManager::load()
{
    if (_settings != NULL)
	{
        delete _settings;
        _settings = NULL;
	}

	// Load settings
    dirs = g_platform->getDirectories();
	portable = QFile::exists(dirs["portableConfig"][0] + iniName);

	qDebug("Loading settings in %s mode from %s", portable ? "portable" : "installed", qPrintable(configDirectory(portable)));
	QString iniPath = configDirectory(portable) + iniName;
    _settings = new QSettings(configDirectory(portable) + iniName, QSettings::IniFormat);
	if (!QFile::exists(iniPath))
	{
		// Ini file doesn't exist, create some defaults and save them to disk
        QList<Directory> directories = g_platform->getDefaultCatalogDirectories();
		writeCatalogDirectories(directories);
	}
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
void SettingsManager::setPortable(bool makePortable, QWidget* parent)
{
	if (makePortable != portable)
	{
		qDebug("Converting to %s mode", makePortable ? "portable" : "installed");

		// Destroy the QSettings object first so it writes any changes to disk
        if (_settings != NULL)
		{
            delete _settings;
            _settings = NULL;
		}

		QString oldDir = configDirectory(portable);
		QString oldIniName = oldDir + iniName;
		QString oldDbName = oldDir + dbName;
		QString oldHistoryName = oldDir + historyName;

		// Copy the settings to the new location
		// and delete the original settings if they are copied successfully
		QString newDir = configDirectory(makePortable);
		if (QFile::copy(oldIniName, newDir + iniName) &&
			QFile::copy(oldDbName, newDir + dbName) &&
			QFile::copy(oldHistoryName, newDir + historyName))
		{
			QFile::remove(oldIniName);
			QFile::remove(oldDbName);
			QFile::remove(oldHistoryName);
		}
		else
		{
			qWarning("Could not convert to %s mode", makePortable ? "portable" : "installed");
			if (makePortable)
			{
                QMessageBox::warning(parent, tr("Launchy"),
					tr("Could not convert to portable mode. Please check you have write access to the %1 directory.").arg(newDir));
			}
		}

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
	QString result = dirs[portable ? "portableConfig" : "config"][0];
	if (profileName.length() > 0)
	{
		result += "/profiles/" + profileName;
	}
	return result;
}


void SettingsManager::setProfileName(const QString& name)
{
	profileName = name;
}


QList<Directory> SettingsManager::readCatalogDirectories()
{
	QList<Directory> result;
    int size = _settings->beginReadArray("directories");
	for (int i = 0; i < size; ++i)
	{
        _settings->setArrayIndex(i);
		Directory tmp;
        tmp.name = _settings->value("name").toString();
		if (tmp.name.length() > 0)
		{
            tmp.types = _settings->value("types").toStringList();
            tmp.indexDirs = _settings->value("indexDirs", false).toBool();
            tmp.indexExe = _settings->value("indexExes", false).toBool();
            tmp.depth = _settings->value("depth", 100).toInt();
			result.append(tmp);
		}
	}
    _settings->endArray();

	return result;
}


void SettingsManager::writeCatalogDirectories(QList<Directory>& directories)
{
    _settings->beginWriteArray("directories");
	for (int i = 0; i < directories.count(); ++i)
	{
		if (directories[i].name.length() > 0)
		{
            _settings->setArrayIndex(i);
            _settings->setValue("name", directories[i].name);
            _settings->setValue("types", directories[i].types);
            _settings->setValue("indexDirs", directories[i].indexDirs);
            _settings->setValue("indexExes", directories[i].indexExe);
            _settings->setValue("depth", directories[i].depth);
		}
	}
    _settings->endArray();
}

void SettingsManager::adjustDonateTime()
{
    _settings->setValue("donateTime", QDateTime::currentDateTime().addDays(21));
}

int SettingsManager::getVersion() const
{
    return _settings->value("version", 0).toInt();
}
void SettingsManager::setVersion(int version)
{
    _settings->setValue("version", version);
}

int SettingsManager::getFadeInTime() const
{
    return _settings->value("GenOps/fadein", 0).toInt();
}

int SettingsManager::getFadeOutTime() const
{
    return _settings->value("GenOps/fadeout", 0).toInt();
}

int SettingsManager::getMaxItemsInHistory() const
{
    return _settings->value("GenOps/maxitemsinhistory", 20).toInt();
}

QStringList SettingsManager::getHistoryFor(const QString& location)
{
    QStringList hist;
    return _settings->value(location, hist).toStringList();
}

int SettingsManager::maxNumberOfResults() const
{
    return _settings->value("GenOps/numresults", 10).toInt();
}

bool SettingsManager::doDecorateText() const
{
    return _settings->value("GenOps/decoratetext", false).toBool();
}

double SettingsManager::getOpaqness() const
{
    return _settings->value("GenOps/opaqueness", 100).toInt() / 100.0;
}

bool SettingsManager::showHiddenFiles() const
{
    return _settings->value("GenOps/showHiddenFiles", false).toBool();
}

int SettingsManager::getCondensedView() const
{
    return _settings->value("GenOps/condensedView", 2).toInt();
}

bool SettingsManager::doAlwaysShow() const
{
    return _settings->value("GenOps/alwaysshow", false).toBool();
}

bool SettingsManager::alwaysOnTop() const
{
    return _settings->value("GenOps/alwaystop", false).toBool();
}

bool SettingsManager::checkForUpdates() const
{
    return _settings->value("GenOps/updatecheck", true).toBool();
}

QString SettingsManager::getSkin() const
{
    return _settings->value("GenOps/skin", SkinDefault).toString();
}
void SettingsManager::setSkin(const QString& skin)
{
    _settings->setValue("GenOps/skin", skin);
}

QPoint SettingsManager::getStoredPosition() const
{
    return _settings->value("Display/pos", QPoint(0,0)).toPoint();
}
void SettingsManager::setStoredPosition(const QPoint& point)
{
    _settings->setValue("Display/pos", point);
}

bool SettingsManager::showTrayIcon() const
{
    return _settings->value("GenOps/showtrayicon", true).toBool();
}

int SettingsManager::getNumViewable() const
{
    return _settings->value("GenOps/numviewable", "4").toInt();
}

bool SettingsManager::hideIfLostFocus() const
{
    return _settings->value("GenOps/hideiflostfocus", false).toBool();
}

int SettingsManager::getAutoSuggestDelay() const
{
    return _settings->value("GenOps/autoSuggestDelay", 1000).toInt();
}
int SettingsManager::getUpdateTimer() const
{
    return _settings->value("GenOps/updatetimer", 10).toInt();
}
int SettingsManager::getAlwaysCenterOption() const
{
    return _settings->value("GenOps/alwayscenter", 3).toInt();
}

int SettingsManager::getHotkey() const
{
    return _settings->value("GenOps/hotkey", -1).toInt();
}

int SettingsManager::getHotkeyModifier(int def) const
{
    return _settings->value("GenOps/hotkeyModifier", def).toInt();
}

int SettingsManager::getHotkeyAction() const
{
     return _settings->value("GenOps/hotkeyAction", Qt::Key_Space).toInt();
}

int SettingsManager::getProxyType() const
{
    return _settings->value("WebProxy/type", 0).toInt();
}
QString SettingsManager::getProxyAddress() const
{
    return _settings->value("WebProxy/hostAddress", "").toString();
}
int SettingsManager::getProxyPort() const
{

}
