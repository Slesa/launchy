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



//#include <QTest>

#include "gcalc.h"
#include <QDebug>
#include <QUrl>





void Process::run()
{
	QString webQuery = "/search?source=launchy&q=";
//	query.replace("?","");	
	query = QUrl::toPercentEncoding(query);
	webQuery += query;
//	qDebug() << webQuery;
	resBuffer.open(QIODevice::ReadWrite);

    connect(&http, SIGNAL(done(bool)), this, SLOT(httpGetFinished(bool)));
	http.setHost("www.google.com");	
	http.get(webQuery, &resBuffer);	
	loop.exec();
} 

void Process::httpGetFinished(bool error) {

	if (!error) {
		result = resBuffer.data();
		QRegExp regex_res("<h2 class=r><font size=\\+1><b>(.*)</b></h2></td>", Qt::CaseInsensitive);
//		qDebug() << result;
		if (regex_res.indexIn(result) != -1) {
		    QLocale locale;
		    result = regex_res.cap(1);
		    result = result.replace("<font size=-2> </font>",locale.groupSeparator());
		    result = result.replace("<sup>",locale.exponential() + QString("("));
			result = result.replace("</sup>",")");
			result = result.replace("&#215;", "x");
			result = result.split("=")[1].trimmed();
/*			QStringList spl = result.split("<b>");
			spl = spl[1].split("</b>");
			result = spl[0];
*/			
		} else {
			result = "unknown";
		}

	} else {
		result = "Error";
	}
	loop.exit();
}



void gcalcPlugin::init()
{
}

void gcalcPlugin::getCatalog(QList<CatItem>* items)
{
	items->push_back(CatItem("GCalc.gcalc", "GCalc", HASH_gcalc, getIcon()));
}

void gcalcPlugin::getID(uint* id)
{
    *id = HASH_gcalc;
}

void gcalcPlugin::getName(QString* str)
{
    *str = "GCalc";
}



void gcalcPlugin::getResults(QList<InputData>* id, QList<CatItem>* results)
{
	if (id->count() != 2)
		return;

	const QString & text = id->first().getText();

	if (!text.contains("gcalc", Qt::CaseInsensitive))
		return;

	const QString & query = id->last().getText();

//	if (!query.contains("?", Qt::CaseInsensitive))
//		return;

	Process p;
	p.query = query;
	p.run();

//	results->clear();
	if (results->size() == 0)
		results->push_front(CatItem(p.result + ".weby", p.result, HASH_gcalc, getIcon()));
}




QString gcalcPlugin::getIcon()
{
    return libPath + "/icons/calcy.png";
}


void gcalcPlugin::setPath(QString * path) {
    libPath = *path;
}

int gcalcPlugin::msg(int msgId, void* wParam, void* lParam)
{
    bool handled = false;
    switch (msgId)
	{		
	case MSG_INIT:
	    init();
	    handled = true;
	    break;
	case MSG_GET_ID:
	    getID((uint*) wParam);
	    handled = true;
	    break;
	case MSG_GET_NAME:
	    getName((QString*) wParam);
	    handled = true;
	    break;
	case MSG_GET_RESULTS:
	    getResults((QList<InputData>*) wParam, (QList<CatItem>*) lParam);
	    handled = true;
	    break;
	case MSG_GET_CATALOG:
		getCatalog((QList<CatItem>*) wParam);
		handled = true;
		break;
	case MSG_PATH:
	    setPath((QString *) wParam);	    
	default:
	    break;
	}
    
    return handled;
}

Q_EXPORT_PLUGIN2(gcalc, gcalcPlugin) 
