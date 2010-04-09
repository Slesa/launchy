/*
Launchy: Application Launcher
Copyright (C) 2009-2010  Simon Capewell, Josh Karlin

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
#include "FileSearch.h"
#include "main.h"
#include "globals.h"


void FileSearch::search(const QString& searchText, QList<CatItem>& searchResults, InputDataList& inputData)
{
	QString searchPath = QDir::fromNativeSeparators(searchText);

	if (searchPath.startsWith("~"))
		searchPath.replace("~", QDir::homePath());

#ifdef Q_WS_WIN
	if (searchPath == "/")
	{
		// Special case for Windows: list available drives
		QFileInfoList driveList = QDir::drives();
		for (int i = driveList.length()-1; i >= 0; --i)
		{
			QFileInfo info = driveList[i];
			// Retrieve volume name
			QString volumeName;
			WCHAR volName[MAX_PATH];
			if (GetVolumeInformation((WCHAR*)info.filePath().utf16(), volName, MAX_PATH, NULL, NULL, NULL, NULL, 0))
				volumeName = QString::fromUtf16((const ushort*)volName);
			else
				volumeName = QDir::toNativeSeparators(info.filePath());
			CatItem item(QDir::toNativeSeparators(info.filePath()), volumeName);
			item.id = HASH_LAUNCHYFILE;
			searchResults.push_front(item);
		}
		return;
	}
	if (searchPath.size() == 2 && searchText[0].isLetter() && searchPath[1] == ':')
		searchPath += "/";
#endif

	// Network searches are too slow to run in the main thread
	if (searchPath.startsWith("//"))
	{
		if (!gSettings->value("GenOps/showNetwork", true).toBool())
			return;

		QRegExp re("//([a-z0-9\\-]+)?$", Qt::CaseInsensitive);
		if (re.exactMatch(searchPath))
		{
			inputData.last().setLabel(LABEL_FILE);

			QList<QString> computers;
			platform->getComputers(computers);

			// Filter computer names by search text
			for (int i = computers.length()-1; i >= 0; --i)
			{
				QString computer = computers[i];
				QString computerPath = "//" + computer;
				if (computerPath.indexOf(searchPath, 0, Qt::CaseInsensitive) == 0)
				{
					CatItem item(QDir::toNativeSeparators(computerPath), computer);
					item.id = HASH_LAUNCHYFILE;
					searchResults.push_front(item);
				}
			}
		}
		return;
	}

	// Split the string on the last slash
	QString directoryPart = searchPath.mid(0,searchPath.lastIndexOf("/")+1);
	QString filePart = searchPath.mid(searchPath.lastIndexOf("/")+1);

	QFileInfo info(directoryPart);
	if (!info.isDir())
		return;

	inputData.last().setLabel(LABEL_FILE);

	// Okay, we have a directory, find files that match "file"
	QDir dir(directoryPart);
	QStringList fileList;
	QDir::Filters filters = QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot;
#ifndef Q_WS_WIN
        filters |= QDir::CaseSensitive;
#endif

	if (gSettings->value("GenOps/showHiddenFiles", false).toBool())
		filters |= QDir::Hidden;

	bool userWildcard = false;
	QString fileSearch;
	if (gSettings->value("GenOps/wildcardFileSearch", false).toBool())
	{
		userWildcard = filePart.contains("*") || filePart.contains("?") || filePart.contains("[");
		fileSearch = filePart;
	}
	fileSearch += "*";

	fileList = dir.entryList(QStringList(fileSearch), filters, QDir::DirsLast | QDir::IgnoreCase | QDir::LocaleAware);

	for (int i = fileList.length()-1; i >= 0; --i)
	{
		QString fileName = fileList[i];
		if (userWildcard || fileName.indexOf(filePart, 0, Qt::CaseInsensitive) == 0)
		{
			QString filePath = dir.absolutePath() + "/" + fileName;
			filePath = QDir::cleanPath(filePath);
			CatItem item(QDir::toNativeSeparators(filePath), fileName);
			item.id = HASH_LAUNCHYFILE;
			searchResults.push_front(item);
		}
	}

	// Showing a directory
	if (filePart.count() == 0)
	{
		QString fullPath = QDir::toNativeSeparators(directoryPart);
		if (!fullPath.endsWith(QDir::separator()))
			fullPath += QDir::separator();
		QString name = info.dir().dirName();
		CatItem item(fullPath, name.count() == 0 ? fullPath : name);
		item.id = HASH_LAUNCHYFILE;
		searchResults.push_front(item);
	}
}
