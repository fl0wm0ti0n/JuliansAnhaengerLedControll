
// 
// 
// 

#include "digitalOut.h"

digitalOut::digitalOut(char n[], int p)
	:actor(digitalOut_act, n, p)
{
#ifdef DEBUG
	static char* const buffer PROGMEM = "Logging1";
	logger_g_ = logger::GetInstance(DEFAULT_LOGLEVEL, DEFAULT_LOGTARGET, buffer);
#endif

	actor::setValue(digitalRead(p));
}

digitalOut::~digitalOut()
{
}

bool digitalOut::setValue(int v)
{
#ifdef DEBUG
	static const char* const buffer PROGMEM = "Call - digitalOut - setValue";
	logger_g_->LnWriteLog(buffer, extremedebug);
#endif

	if (getValue() != v)
	{
		actor::setValue(v);
		digitalWrite(getPin(),v);
	}
	return true;
}

bool digitalOut::doggle()
{
#ifdef DEBUG
	static const char* const buffer PROGMEM = "Call - digitalOut - doggle";
	logger_g_->LnWriteLog(buffer, extremedebug);
#endif

	return setValue(!getValue());
}