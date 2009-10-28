#ifndef ICONCACHE_H
#define ICONCACHE_H


class IconCache : public QObject
{
	Q_OBJECT

public:
	IconCache(const QString& path);

	QString getIconPath(const QString& site);

public slots:
	void finished();

private:
	QNetworkReply* query(const QUrl& url, int depth = 0);

	QString cachePath;
	QNetworkAccessManager nam;
	QEventLoop loop;
};


#endif // ICONCACHE_H
