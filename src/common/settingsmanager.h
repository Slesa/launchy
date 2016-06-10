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
#include <QObject>

class QSettings;

class LoadablePlugins : public QHash<uint, bool> {};

class SettingsManager : public QObject
{
    Q_OBJECT
public:
    static const char* SkinDefault;

public:
    SettingsManager();
    QSettings* getQSettings();
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

    LoadablePlugins readLoadablePlugins();
    void writeLoadablePlugins(LoadablePlugins& plugins);

    // Settings properties
    void adjustDonateTime();
    QDateTime getDonateTime(const QDateTime& def) const;
    void setDonateTime(const QDateTime& donateTime);

    int getVersion() const;
    void setVersion(int version);

    void setHistory(const QString& location, const QStringList& history);

    int getFadeInTime() const;
    void setFadeInTime(int time);
    int getFadeOutTime() const;
    void setFadeOutTime(int time);

    int getMaxItemsInHistory() const;
    void setMaxItemsInHistory(int max);

    QStringList getHistoryFor(const QString& location);
    int maxNumberOfResults() const;
    void setMaxNumberOfResults(int max);

    bool doDecorateText() const;
    void setDoDecorateText(bool on);

    double getOpaqness() const;
    void setOpaqness(double value);

    bool showHiddenFiles() const;
    void setShowHiddenFiles(int show);

    bool showNetwork() const;
    void setShowNetwork(bool show);

    int getCondensedView() const;
    void setCondesedView(int view);

    bool doAlwaysShow() const;
    void setAlwaysShow(bool show);

    bool alwaysOnTop() const;
    void setAlwaysOnTop(bool on);

    bool checkForUpdates() const;
    void setCheckForUpdates(bool on);

    QString getSkin() const;
    void setSkin(const QString& skin);

    QPoint getStoredPosition() const;
    void setStoredPosition(const QPoint& point);

    bool showTrayIcon() const;
    void setShowTrayIcon(bool show);

    int getNumViewable() const;
    void setNumViewable(int view);

    bool hideIfLostFocus() const;
    void setHideIfLostFocus(bool on);

    int getAutoSuggestDelay() const;
    void setAutoSuggestDelay(int delay);

    int getUpdateTimer() const;
    void setUpdateTimer(int delay);

    int getAlwaysCenterOption() const;
    void setAlwaysCenterOption(int option);

    int getDragMode() const;
    void setDragMode(int mode);

    int getHotkey() const;
    void setHotkey(int key);
    int getHotkeyModifier(int def) const;
    int getHotkeyAction() const;

    int getProxyType() const;
    QString getProxyAddress() const;
    void setProxyAddress(const QString& address);
    int getProxyPort() const;
    void setProxyPort(int port);

private:
	QString configDirectory(bool portable) const;

private:
    bool portable;
	QString profileName;
    QHash<QString, QList<QString> > dirs;
    QSettings* _settings;
};


#endif
