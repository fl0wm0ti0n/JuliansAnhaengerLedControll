#include "digitalIn.h"

digitalIn::digitalIn(String n, int p)
	:sensor(digitalIn_sens, n, p)
{
	logger_g_ = logger::GetInstance(DEFAULT_LOGLEVEL, DEFAULT_LOGTARGET, "Logging1");
	sensor::getValue(digitalRead(p));
	setPin(p);
}

digitalIn::~digitalIn()
= default;

bool digitalIn::getValue()
{
	logger_g_->WriteLog(F("Call - digitalIn.getValue"), extremedebug);
	if (digitalRead(getPin()) == HIGH)
	{
		logger_g_->WriteLog("Switch: is true", sensordata);
		bSwitchState_ = true;
	}
	else
	{
		logger_g_->WriteLog("Switch: is false", sensordata);
		bSwitchState_ = false;
	}
	return bSwitchState_;
}