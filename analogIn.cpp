// @file      analogIn.cpp
// @author    flow@p0cki.net
// @date      08.2019
// @brief     childclass of sensor

#include "analogIn.h"

analogIn::analogIn(char n[], int p)
	:sensor(analogIn_sens, n, p)
{
#ifdef DEBUG
	static char* const buffer PROGMEM = "Logging1";
	logger_g_ = logger::GetInstance(DEFAULT_LOGLEVEL, DEFAULT_LOGTARGET, buffer);
#endif

	sensor::getValue(analogRead(p));
	setPin(p);
}

analogIn::~analogIn()
= default;

int analogIn::getValue()
{
#ifdef DEBUG
	static const char* const buffer PROGMEM = "Call - analogIn.getValue";
	logger_g_->WriteLog(buffer, extremedebug);
#endif

	iAnalogValue = analogRead(iPinNum);
	return iAnalogValue;
}