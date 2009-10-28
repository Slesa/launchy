#include "precompiled.h"
#include "IconCache.h"


IconCache::IconCache(const QString& path) :
	cachePath(path)
{
}


QNetworkReply* IconCache::query(const QUrl& url, int depth)
{
	QNetworkRequest request;
	request.setUrl(url);
	QNetworkReply* reply = nam.get(request);
	QObject::connect(reply, SIGNAL(finished()), this, SLOT(finished()));

	qDebug() << "Entering icon loop " << url.toString();
	loop.exec();
	qDebug() << "Leaving icon loop " << url.toString();

	QVariant redirectionTarget = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
	if (redirectionTarget.isValid())
	{
		delete reply;
		reply = depth < 5 ? query(redirectionTarget.toUrl(), depth + 1) : NULL;
	}

	return reply;
}


QString IconCache::getIconPath(const QString& site)
{
	QString cachedName = site;
	cachedName = cachedName.replace("http:", "").replace("https:", "").replace("/", "");
	QFileInfo info;
	info.setFile(cachePath, cachedName + ".png");
	if (info.exists())
		return info.size() > 0 ? info.absoluteFilePath() : QString();
	info.setFile(cachePath, cachedName + ".ico");
	if (info.exists())
		return info.size() > 0 ? info.absoluteFilePath() : QString();

	if (!site.startsWith("http"))
		return "";

	QFile file(cachePath + cachedName + ".ico");
	if (!file.open(QIODevice::WriteOnly))
	{
		qDebug() << "Could not open icon for writing";
		return "";
	}

	QNetworkReply* reply = query(QUrl(site + "/favicon.ico"));
	if (reply && reply->error() != QNetworkReply::NoError)
	{
		QByteArray ba = reply->readAll();
		file.write(ba);

		delete reply;
		return file.fileName();
	}

	return "";
}


void IconCache::finished()
{
//	QNetworkReply* reply = qobject_cast<QNetworkReply*>(this->sender());

	loop.exit();
}
