#ifndef FADER_H
#define FADER_H


#include <QThread>
#include <QMutex>


class Fader : public QThread
{
	Q_OBJECT

public:
	Fader(QObject* parent = NULL);
	~Fader();
	
	void fadeIn(bool quick);
	void fadeOut(bool quick);
	void run();

	inline void stop() { keepRunning = false; }
	inline bool isFading() const { return delta < 0 && isRunning(); }

signals:
	void fadeLevel(double level);

private:
	QMutex mutex;
	bool keepRunning;

	double delta;
	int delay;

	double level;
	double targetLevel;
};


#endif
