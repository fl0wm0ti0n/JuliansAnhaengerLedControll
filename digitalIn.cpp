#include "digitalIn.h"

digitalIn::digitalIn(char n[], unsigned short p)
	:sensor(digitalIn_sens, n, p)
{
#ifdef DEBUG
	static char* const buffer PROGMEM = "Logging1";
	logger_g_ = logger::GetInstance(DEFAULT_LOGLEVEL, DEFAULT_LOGTARGET, buffer);
#endif
	setPin(p);
	set_debounce_interval(DEFAULT_DEBOUNCE);
}

digitalIn::~digitalIn()
= default;

 bool digitalIn::setPin(unsigned short p)
{
	 input_ = Bounce();
	 input_.attach(p, INPUT_PULLUP);

	return true;
}

 void digitalIn::update_debounce()
 {
	 input_.update();
 }

 void digitalIn::set_debounce_interval(unsigned short interval)
 {
	 input_.interval(interval);
 }

bool digitalIn::getValue()
{
#ifdef DEBUG
	static const char* const buffer PROGMEM = "Call - digitalIn.getValue";
	logger_g_->LnWriteLog(buffer, extremedebug);
#endif

	if (input_.read() == HIGH)
	{
		//Serial.println(F("Switch: is true"));
#ifdef DEBUG
		static const char* const buffer PROGMEM = "Switch: is true";
		logger_g_->LnWriteLog(buffer, sensordata);
#endif
		bSwitchState_ = true;
	}
	else
	{
		//Serial.println(F("Switch: is false"));
#ifdef DEBUG

		static const char* const buffer PROGMEM = "Switch: is false";
		logger_g_->LnWriteLog(buffer, sensordata);
#endif
		bSwitchState_ = false;
	}
	return bSwitchState_;
}

bool digitalIn::getFell()
{
#ifdef DEBUG
	static const char* const buffer PROGMEM = "Call - digitalIn.getFell";
	logger_g_->LnWriteLog(buffer, extremedebug);
#endif
	return input_.fell();
}

bool digitalIn::getRose()
{
#ifdef DEBUG
	static const char* const buffer PROGMEM = "Call - digitalIn.getRose";
	logger_g_->LnWriteLog(buffer, extremedebug);
#endif
	return input_.rose();
}

bool digitalIn::getChanged()
{
#ifdef DEBUG
	static const char* const buffer PROGMEM = "Call - digitalIn.getRose";
	logger_g_->LnWriteLog(buffer, extremedebug);
#endif
	return input_.changed();
}