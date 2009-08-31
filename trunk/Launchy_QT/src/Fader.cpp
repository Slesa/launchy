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


void Fader::fadeIn()
{
	int time = gSettings->value("GenOps/fadein", 0).toInt();
	double end = (double) gSettings->value("GenOps/opaqueness", 100).toInt() / 100.0;

	if (time != 0)
	{
		double delay = ((double) time) / (end / 0.05);

		for (double i = 0.0; i < end + 0.01 && keepRunning; i += 0.05)
		{
			emit fadeLevel(i);
			msleep(delay);
		}
	}

	emit fadeLevel(end);
	emit finishedFade(end);
}


void Fader::fadeOut()
{
	int time = gSettings->value("GenOps/fadeout", 0).toInt();

	if (time != 0)
	{
		double start = (double) gSettings->value("GenOps/opaqueness", 100).toInt() / 100.0;
		double delay = ((double) time) / (start / 0.05);

		for (double i = start; i > -0.01 && keepRunning; i -= 0.05)
		{
			emit fadeLevel(i);
			msleep(delay);
		}
	}

	emit fadeLevel(0.0);
	emit finishedFade(0.0);
}


void Fader::run()
{
	keepRunning = true;
	if (fadeType)
		fadeIn();
	else
		fadeOut();
}
