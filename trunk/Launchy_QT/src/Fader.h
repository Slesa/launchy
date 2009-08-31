#ifndef FADER_H
#define FADER_H


#include <QThread>


class Fader : public QThread
{
	Q_OBJECT

public:
	Fader(QObject* parent = NULL);
	~Fader();
	void stop() { keepRunning = false; }
	void run();
	void fadeIn();
	void fadeOut();
	void setFadeType(bool type);
signals:
	void fadeLevel(double);
	void finishedFade(double);

private:
	bool keepRunning;
	bool fadeType;
};


inline void Fader::setFadeType(bool type)
{
	fadeType = type;
}


#endif
