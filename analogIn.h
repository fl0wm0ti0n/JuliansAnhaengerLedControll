// analogIn.h

#ifndef _HOME_AIN_h
#define _HOME_AIN_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "sensor.h"

class analogIn final : public sensor
{
	private:
		int iAnalogValue;
		int	iPinNum;
		logger* logger_g_;

	public:
		analogIn(char n[], int p);			// Constructor
		~analogIn();						// Destructor

		int getValue();						// Method: get Value
};

#endif