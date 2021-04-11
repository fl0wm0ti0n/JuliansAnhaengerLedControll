#include "digitalIn.h"

digitalIn::digitalIn(char n[], int p)
	:sensor(digitalIn_sens, n, p)
{
#ifdef DEBUG
	static char* const buffer PROGMEM = "Logging1";
	logger_g_ = logger::GetInstance(DEFAULT_LOGLEVEL, DEFAULT_LOGTARGET, buffer);
#endif
	sensor::getValue(digitalRead(p));
	setPin(p);
}

digitalIn::~digitalIn()
= default;

bool digitalIn::getValue()
{
#ifdef DEBUG
	static const char* const buffer PROGMEM = "Call - digitalIn.getValue";
	logger_g_->LnWriteLog(buffer, extremedebug);
#endif

	if (digitalRead(getPin()) == HIGH)
	{
		Serial.println(F("Switch: is true"));
#ifdef DEBUG
		static const char* const buffer PROGMEM = "Switch: is true";
		logger_g_->LnWriteLog(buffer, sensordata);
#endif
		bSwitchState_ = true;
	}
	else
	{
#ifdef DEBUG
		static const char* const buffer PROGMEM = "Switch: is false";
		logger_g_->LnWriteLog(buffer, sensordata);
#endif
		bSwitchState_ = false;
	}
	return bSwitchState_;
}