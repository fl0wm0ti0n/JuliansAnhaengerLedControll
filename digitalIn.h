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
#include <Bounce2.h>

class digitalIn final :public sensor
{
private:
	bool	bSwitchState_ = false;
	logger* logger_g_;
	Bounce input_;
	
public:
	digitalIn(char n[], unsigned short p);
	~digitalIn();
	bool getFell();
	bool getRose();
	bool getChanged();
	virtual bool getValue();
	virtual bool setPin(unsigned short p);
	void update_debounce();
	void set_debounce_interval(unsigned short interval);
};


#endif

