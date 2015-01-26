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

#include "win_launchywidget.h"
#include "win_files.h"
#include "win_util.h"

WinLaunchyWidget::WinLaunchyWidget(CommandFlags command) :
    LaunchyWidget(command)
{
    commandMessageId = RegisterWindowMessage(_T("LaunchyCommand"));
}

bool WinLaunchyWidget::nativeEvent(const QByteArray& eventType, void* message, long* result)
{
    MSG* msg = (MSG*) message;
    switch (msg->message)
    {
    case WM_SETTINGCHANGE:
        // Refresh Launchy's environment on settings changes
        if (msg->lParam && _tcscmp((TCHAR*)msg->lParam, _T("Environment")) == 0)
        {
            UpdateEnvironment();
        }
        break;

    case WM_ENDSESSION:
        // Ensure settings are saved
        saveSettings();
        break;

    // Might need to capture these two messages if Vista gives any problems with alpha borders
    // when restoring from standby
    case WM_POWERBROADCAST:
        break;
    case WM_WTSSESSION_CHANGE:
        break;

    default:
        if (msg->message == commandMessageId)
        {
    // A Launchy startup command
            executeStartupCommand(msg->wParam);
        }
        break;
    }
    return LaunchyWidget::nativeEvent(eventType, msg, result);
}


