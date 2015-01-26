/*
Launchy: Application Launcher
Copyright (C) 2007-2009  Josh Karlin, Simon Capewell

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
#ifndef WIN_LAUNCHYWIDGET
#define WIN_LAUNCHYWIDGET

#include "LaunchyWidget.h"

// Override the main widget to handle incoming system messages. We could have done this in the QApplication 
// event handler, but then we'd have to filter out the duplicates for messages like WM_SETTINGCHANGE.
class WinLaunchyWidget : public LaunchyWidget
{
public:
    WinLaunchyWidget(CommandFlags command);
    virtual bool nativeEvent(const QByteArray& eventType, void* message, long* result);

private:
	UINT commandMessageId;
};

#endif
