#ifndef PLATFORM_BASE_H
#define PLATFORM_BASE_H
#include <QWidget>
#include <QPoint>
#include <QStringList>
#include <QList>
#include <QFileIconProvider>

#include "options.h"

#include "platform_base_hotkey.h"
#include "platform_Base_hottrigger.h"

class PlatformBase {
public:
	PlatformBase() {
		icons = NULL;
	}
	virtual ~PlatformBase() {
	}

	QFileIconProvider* icons;


	virtual QString GetSettingsDirectory() = 0;
	virtual QList<Directory> GetInitialDirs() = 0;
	virtual void AddToNotificationArea() = 0;
	virtual void RemoveFromNotificationArea() = 0;
	virtual void Execute(QString path, QString args) = 0;


	QKeySequence oldKey;

	// Set hotkey
	void SetHotkey(const QKeySequence& key, QObject* receiver, const char* slot) {
		GlobalShortcutManager::disconnect(oldKey, receiver, slot);
		GlobalShortcutManager::connect(key, receiver, slot);
		oldKey = key;
	}


	// Alpha border functions	
	virtual bool CreateAlphaBorder(WId id, QString ImageName) { return false; }
	virtual void DestroyAlphaBorder() { return; }
	virtual void MoveAlphaBorder(QPoint pos) { return; }
	virtual void HideAlphaBorder() { return; }
	virtual void ShowAlphaBorder() { return; }


};

/*
class LaunchyIconProvider2 : friend QFileIconProvider {
	LaunchyIconProvider2() : QFileIconProvider() {}
//	~LaunchyIconProvider() : ~QFileIconProvider() {}
	QIcon icon ( const QFileInfo & info ) const;

};
*/
#endif