// @file           analogOut.cpp
// @author         flow@p0cki.net
// @date           08.2019
// @brief          class for analog output

#include "analogOut.h"
#include "logger.h"

analogOut::analogOut(char n[], int p)
	:actor(analogOut_act, n, p)
{
#ifdef DEBUG
	static char* const buffer PROGMEM = "Logging1";
	logger_g_ = logger::GetInstance(DEFAULT_LOGLEVEL, DEFAULT_LOGTARGET, buffer);
#endif
	actor::setValue(analogRead(p));
}

analogOut::~analogOut()
= default;

bool analogOut::setValue(int v)
{
#ifdef DEBUG
	static const char* const buffer PROGMEM = "Call - analogOut - setValue";
	logger_g_->LnWriteLog(buffer, extremedebug);
#endif


	if (getValue() != v)
	{
		actor::setValue(v);
		analogWrite(getPin(), v);
	}
	return true;
}

bool analogOut::doggle()
{
	static const char* const buffer PROGMEM = "Call - analogOut - doggle";
	logger_g_->LnWriteLog(buffer, extremedebug);
	return setValue(!getValue());
}

void analogOut::SlowlyIncreaseOrDecreaseValue(bool sensorResult, int maxValue)
{
#ifdef DEBUG
	static const char* const buffer PROGMEM = "Call - LEDSlowlyInDecreaseValue";
	logger_g_->WriteLog(buffer, extremedebug);
#endif
	if (sensorResult)
	{
		if (lightcounter_ != maxValue)
		{
			if (lightcounter_ < maxValue)
			{
				lightcounter_++;
#ifdef DEBUG
				static const char* const buffer PROGMEM = "Value Up - ";
				logger_g_->LnWriteLog(buffer, extremedebug);
				logger_g_->WriteLog(lightcounter_, extremedebug);
#endif

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
#ifdef DEBUG
				static const char* const buffer PROGMEM = "Value Down - ";
				logger_g_->LnWriteLog(buffer, extremedebug);
				logger_g_->WriteLog(lightcounter_, extremedebug);
#endif
			}
		}
	}
	setValue(lightcounter_);
}