// @file      analogIn.cpp
// @author    flow@p0cki.net
// @date      08.2019
// @brief     childclass of sensor

#include "analogIn.h"

analogIn::analogIn(String n, int p)
	:sensor(analogIn_sens, n, p)
{
	logger_g_ = logger::GetInstance(DEFAULT_LOGLEVEL, DEFAULT_LOGTARGET, "Logging1");
	sensor::getValue(analogRead(p));
	setPin(p);
}

analogIn::~analogIn()
= default;

int analogIn::getValue()
{
	logger_g_->WriteLog(F("Call - analogIn.getValue"), extremedebug);
	iAnalogValue = analogRead(iPinNum);
	return iAnalogValue;
}