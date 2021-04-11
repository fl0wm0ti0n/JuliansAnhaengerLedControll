#pragma once
#ifndef _DIGITALIN_h
#define _DIGITALIN_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include "logger.h"
#include "sensor.h"

class digitalIn final :public sensor
{
private:
	bool	bSwitchState_ = false;
	logger* logger_g_;
	
public:
	digitalIn(char n[], int p);
	~digitalIn();

	bool getValue();	// Method: get Value
};

#endif

