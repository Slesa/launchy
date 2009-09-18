
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


#include "precompiled.h"
#include "icon_extractor.h"
#include "globals.h"
#include "main.h"


IconExtractor::IconExtractor()
{
}


void IconExtractor::processIcon(CatItem item, bool highPriority)
{
	mutex.lock();

	item.id = -1;
	if (highPriority)
		items.push_front(item);
	else
		items.push_back(item);

	mutex.unlock();

	if (!isRunning())
		start(IdlePriority);
}


void IconExtractor::processIcons(const QList<CatItem>& newItems, bool reset)
{
	mutex.lock();

	if (reset && isRunning())
		items.clear();

	items += newItems;
	for (int i = 0; i < items.size(); ++i)
		items[i].id = i;

	mutex.unlock();

	if (!isRunning())
		start(IdlePriority);
}


void IconExtractor::stop()
{
	mutex.lock();
	items.clear();
	mutex.unlock();
}


void IconExtractor::run()
{
#ifdef Q_WS_WIN
	CoInitialize(NULL);
#endif

	CatItem item;
	bool itemsRemaining = true;

	do
	{
		mutex.lock();
		itemsRemaining = items.size() > 0;
		if (itemsRemaining)
			item = items.dequeue();
		mutex.unlock();
		if (itemsRemaining)
			emit iconExtracted(item.id, getIcon(item));
	}
	while (itemsRemaining);

#ifdef Q_WS_WIN
	CoUninitialize();
#endif
}


QIcon IconExtractor::getIcon(const CatItem& item)
{
	if (item.icon.isNull())
	{
#ifdef Q_WS_X11
		QFileInfo info(item.fullPath);
		if (info.isDir())
			return platform->icon(QFileIconProvider::Folder);
#endif
		return platform->icon(QDir::toNativeSeparators(item.fullPath));
	}
	else
	{
#ifdef Q_WS_X11
		if (QFile::exists(item.icon))
		{
			return QIcon(item.icon);
		}
#endif
		return platform->icon(QDir::toNativeSeparators(item.icon));
	}
}
