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
		unsigned short iAnalogValue;
		unsigned short	iPinNum;
		logger* logger_g_;

	public:
		analogIn(char n[], unsigned short p);			// Constructor
		~analogIn();						// Destructor

		unsigned short getAnalogValue();						// Method: get Value
};

#endif