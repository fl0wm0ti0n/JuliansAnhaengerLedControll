// Motionsensor_one.h

#ifndef _DIRECTIONENCODER_h
#define _DIRECTIONENCODER_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include "sensor.h"
#include "logger.h"

class directionEncoder final : public sensor
{
private:

	byte	pinCLK_;	// Connected to CLK on KY-040
	byte	pinDT_;		// Connected to DT on KY-040
	byte	pinSW_;		// Connected to DT on KY-040
	int		encoderPosCount_ = 0;
	byte	oldCLKVal_;
	byte	CLKVal_;
	bool	bCW_			= false;
	bool	bSwitchState_	= false;
	bool	bdoggleState_	= false;
	int		turn_			= 0;
	byte	oldTurn_		= 0;
	float	clicks_			= 0.5;
	byte	iSwitchState_	= 0;
	bool	iSwitchValue_	= 0;
	logger* logger_g_;

public:
	directionEncoder(char n[], int pSW, int pCLK, int pDT);
	~directionEncoder();
	int getEncoderValue();		// Method: get encoder Value
	bool getValue();				// Method: get switch Value
	bool getSwitchValueDoggle();
	bool getSwitchLongValue();

	void setClickValue(float clicks)
	{
		clicks_ = clicks;
	}
};
#endif