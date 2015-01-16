/*
Launchy: Application Launcher
Copyright (C) 2007-2010  Josh Karlin, Simon Capewell

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
#include "launchywidget.h"
#include "commandlineparser.h"
#include "singleapplication.h"
/*
#include <QWidget>
#include <QScrollBar>
#include <QDesktopWidget>
#include <QMenu>
#include <QMessageBox>
#include <QNetworkRequest>
#include <QNetworkProxy>
#include <QDesktopServices>
#ifdef Q_OS_MAC
//#include <QMacStyle>
#endif
#include "icon_delegate.h"
#include "main.h"
#include "globals.h"
#include "options.h"
#include "plugin_interface.h"
#include "FileSearch.h"
*/

static SingleApplication* _singleApplication;

LaunchyWidget* createLaunchyWidget(CommandFlags command);
void fileLogMsgHandler(QtMsgType type, const QMessageLogContext& context, const QString& msg);

int main(int argc, char *argv[])
{
	createApplication(argc, argv);

	qApp->setQuitOnLastWindowClosed(false);

    QCoreApplication::setApplicationName("Launchy");
    QCoreApplication::setOrganizationDomain("Launchy");
    QCoreApplication::setApplicationVersion(LAUNCHY_VERSION_STRING);

    QString locale = QLocale::system().name();
    QTranslator translator;
    translator.load(QString("tr/launchy_" + locale));
    qApp->installTranslator(&translator);

    CommandLineParser parser;
    parser.process(*qApp);

    CommandFlags command = None;
    bool allowMultipleInstances = parser.doMultiple();

    QString profile = parser.getProfile();
    if(!profile.isEmpty())
        settings.setProfileName(profile);

    if(parser.doLog())
    {
        qInstallMessageHandler(fileLogMsgHandler);
    }

    if(parser.doRescue())
    {
        command = ResetSkin | ResetPosition | ShowView;
    } else
    if(parser.doShow())
    {
        command |= ShowView;
    } else
    if(parser.doOptions())
    {
        command |= ShowOptions;
    }
    if(parser.doRescan())
    {
        command |= Rescan;
    }
    if(parser.doExit())
    {
        command |= Exit;
    }

    if (!allowMultipleInstances)
    {
        _singleApplication = SingleApplication::instance();
        if(!_singleApplication->start())
        {
            _singleApplication->release();
            qDebug() << "Already running";
            platform->sendInstanceCommand(command);
            exit(1);
        }
    }

    qApp->setStyleSheet("file:///:/resources/basicskin.qss");

#ifdef Q_OS_WIN
	LaunchyWidget* widget = createLaunchyWidget(command);
#else
	LaunchyWidget* widget = new LaunchyWidget(command);
#endif

    int result = qApp->exec();

    if(_singleApplication!=NULL)
    {
        _singleApplication->release();
    }

	delete widget;
	widget = NULL;

	delete platform;
	platform = NULL;

    return result;
}

void fileLogMsgHandler(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
    static QTextStream* st = NULL;
    if(st==NULL)
    {
        QString fileName = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + "/graunchy.log";
        QFile fh(fileName);
        if(!fh.open(QIODevice::WriteOnly | QIODevice::Text))
            return;
        st = new QTextStream(&fh);
    }

    switch (type)
    {
    case QtDebugMsg:
        *st << "Debug: " << msg << "(" << context.file << ":" << context.line << ", " << context.function << ")";
        break;
    case QtWarningMsg:
        *st << "Warning: " << msg << "(" << context.file << ":" << context.line << ", " << context.function << ")";
        break;
    case QtCriticalMsg:
        *st << "Critical: " << msg << "(" << context.file << ":" << context.line << ", " << context.function << ")";
        break;
    case QtFatalMsg:
        *st << "Fatal: " << msg << "(" << context.file << ":" << context.line << ", " << context.function << ")";
        abort();
        break;
//        fflush(file);
    }
}
