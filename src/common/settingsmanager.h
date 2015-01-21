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
#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include "directory.h"
#include <QString>
#include <QHash>

class QSettings;

class SettingsManager : public QObject
{
public:
    static const char* SkinDefault;

public:
	SettingsManager();
	void load();

	bool isPortable() const;
	QList<QString> directory(QString name) const;
	QString catalogFilename() const;
	QString historyFilename() const;
	QString skinPath(const QString& skinName) const;
    void setPortable(bool makePortable, QWidget* parent);
	void removeAll();
	void setProfileName(const QString& name);
    QList<Directory> readCatalogDirectories();
    void writeCatalogDirectories(QList<Directory>& directories);

    // Settings properties
    void adjustDonateTime();

    int getVersion() const;
    void setVersion(int version);

    int getFadeInTime() const;
    int getFadeOutTime() const;
    int getMaxItemsInHistory() const;
    QStringList getHistoryFor(const QString& location);
    int maxNumberOfResults() const;
    bool doDecorateText() const;
    double getOpaqness() const;
    bool showHiddenFiles() const;
    int getCondensedView() const;
    bool doAlwaysShow() const;
    bool alwaysOnTop() const;
    bool checkForUpdates() const;

    QString getSkin() const;
    void setSkin(const QString& skin);

    QPoint getStoredPosition() const;
    void setStoredPosition(const QPoint& point);

    bool showTrayIcon() const;
    int getNumViewable() const;
    bool hideIfLostFocus() const;
    int getAutoSuggestDelay() const;
    int getUpdateTimer() const;
    int getAlwaysCenterOption() const;

    int getHotkey() const;
    int getHotkeyModifier(int def) const;
    int getHotkeyAction() const;

    int getProxyType() const;
    QString getProxyAddress() const;
    int getProxyPort() const;

private:
	QString configDirectory(bool portable) const;

private:
    bool portable;
	QString profileName;
    QHash<QString, QList<QString> > dirs;
    QSettings* _settings;
};


#endif
