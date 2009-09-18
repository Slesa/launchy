#include "precompiled.h"
#include "Fader.h"
#include "globals.h"


Fader::Fader(QObject* parent) :
	QThread(parent),
	keepRunning(true)
{
}


Fader::~Fader()
{
}


void Fader::fadeIn(bool quick)
{
	int time = gSettings->value("GenOps/fadein", 0).toInt();

	mutex.lock();
	targetLevel = gSettings->value("GenOps/opaqueness", 100).toInt() / 100.0;
	delta = 0.05;
	delay = quick ? 0 : time * delta / targetLevel;
	if (delay > 10)
	{
		delta /= 10;
		delay /= 10;
	}
	mutex.unlock();

	if (quick)
	{
		// stop any current slow fades
		stop();
		wait();
		emit fadeLevel(targetLevel);
	}
	else if (!isRunning())
	{
		level = 0;
		start();
	}
}


void Fader::fadeOut(bool quick)
{
	int time = gSettings->value("GenOps/fadeout", 0).toInt();
	double opaqueness = gSettings->value("GenOps/opaqueness", 100).toInt() / 100.0;

	mutex.lock();
	targetLevel = 0;
	delta = -0.05;
	delay = quick ? 0 : time * -delta / opaqueness;
	if (delay > 10)
	{
		delta /= 10;
		delay /= 10;
	}
	mutex.unlock();

	// qDebug() << level << " to " << targetLevel << " delay " << delay;

	if (quick)
	{
		// stop any current slow fades
		stop();
		wait();
		emit fadeLevel(targetLevel);
	}
	else if (!isRunning())
	{
		level = opaqueness;
		start();
	}
}


void Fader::run()
{
	keepRunning = true;

	while (keepRunning)
	{
		mutex.lock();
		level += delta;
		keepRunning &= (delta > 0 && level < targetLevel) || (delta < 0 && level > targetLevel);
		mutex.unlock();

		// qDebug() << delta << level << targetLevel << keepRunning;

		if (keepRunning)
		{
			emit fadeLevel(level);
			msleep(delay);
		}
	}

	// qDebug() << delta << targetLevel << targetLevel;
	// qDebug() << "";

	emit fadeLevel(targetLevel);
}
