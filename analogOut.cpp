// @file           analogOut.cpp
// @author         flow@p0cki.net
// @date           08.2019
// @brief          class for analog output

#include "analogOut.h"
#include "logger.h"

analogOut::analogOut(String n, int p)
	:actor(analogOut_act, n, p)
{
	logger_g_ = logger::GetInstance(DEFAULT_LOGLEVEL, DEFAULT_LOGTARGET, "Logging1");
	actor::setValue(analogRead(p));
}

analogOut::~analogOut()
= default;

bool analogOut::setValue(int v)
{
	logger_g_->WriteLog("Call - analogOut - setValue", extremedebug);

	if (getValue() != v)
	{
		actor::setValue(v);
		analogWrite(getPin(), v);
	}
	return true;
}

bool analogOut::doggle()
{
	logger_g_->WriteLog("Call - analogOut - doggle", extremedebug);
	return setValue(!getValue());
}

void analogOut::SlowlyIncreaseOrDecreaseValue(bool sensorResult, int maxValue)
{
	logger_g_->WriteLog("Call - LEDSlowlyInDecreaseValue", extremedebug);

	if (sensorResult)
	{
		if (lightcounter_ != maxValue)
		{
			if (lightcounter_ < maxValue)
			{
				lightcounter_++;
				logger_g_->WriteLog("Value Up - " + String(lightcounter_), extremedebug);
			}
		}
	}
	else
	{
		if (lightcounter_ != 0)
		{
			if (lightcounter_ > 0)
			{
				lightcounter_--;
				logger_g_->WriteLog("Value Down - " + String(lightcounter_), extremedebug);
			}
		}
	}
	setValue(lightcounter_);
}