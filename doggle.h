
// @file           doggle.h
// @author         flow@p0cki.at
// @date           12.2016
// @brief          flip a Value

#ifndef _DOGGLE_h
#define _DOGGLE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class doggle
{
private:
	static int iflip;
	static int iflop;
	bool iReturnValue;

public:
	doggle();						// Constructor
	virtual ~doggle();				// Destructor

	int flip(int iValue);				// Method: get on or off					
};
#endif

