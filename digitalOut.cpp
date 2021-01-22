
// 
// 
// 

#include "digitalOut.h"

digitalOut::digitalOut(String n, int p)
	:actor(digitalOut_act, n, p)
{
	logger_g_ = logger::GetInstance(DEFAULT_LOGLEVEL, DEFAULT_LOGTARGET, "Logging1");
	actor::setValue(digitalRead(p));
}

digitalOut::~digitalOut()
{
}

bool digitalOut::setValue(int v)
{
	logger_g_->WriteLog("Call - digitalOut - setValue", extremedebug);

	if (getValue() != v)
	{
		actor::setValue(v);
		digitalWrite(getPin(),v);
	}
	return true;
}

bool digitalOut::doggle()
{
	logger_g_->WriteLog("Call - digitalOut - doggle", extremedebug);
	return setValue(!getValue());
}