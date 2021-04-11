#include "directionEncoder.h"

directionEncoder::directionEncoder(char n[], int pSW, int pCLK, int pDT)
	:sensor(digitalIn_sens, n, pSW)
{
#ifdef DEBUG
	static char* const buffer PROGMEM = "Logging1";
	logger_g_ = logger::GetInstance(DEFAULT_LOGLEVEL, DEFAULT_LOGTARGET, buffer);
#endif
	pinCLK_ = pCLK;
	pinDT_ = pDT;
	pinSW_ = pSW;
	pinMode(pinCLK_, INPUT);
	pinMode(pinDT_, INPUT);
	pinMode(pinSW_, INPUT);
	oldCLKVal_ = digitalRead(pinCLK_);
}

directionEncoder::~directionEncoder()
= default;

int directionEncoder::getEncoderValue()
{
#ifdef DEBUG
	static const char* const buffer PROGMEM = "Call - GetEncoder";
	logger_g_->LnWriteLog(buffer, extremedebug);
#endif
	CLKVal_ = digitalRead(pinCLK_);
	if (CLKVal_ != oldCLKVal_) { // Means the knob is rotating
	  // if the knob is rotating, we need to determine direction
	  // We do that by reading pin B.

		if (digitalRead(pinDT_) != CLKVal_) {  // Means pin A Changed first - We're Rotating Clockwise
			encoderPosCount_ = encoderPosCount_ + clicks_;
			bCW_ = true;
		}
		else {// Otherwise B changed first and we're moving CCW
			bCW_ = false;
			encoderPosCount_ = encoderPosCount_ - clicks_;
		}
#ifdef DEBUG
		static const char* const buffer PROGMEM = "Rotated ";
		logger_g_->LnWriteLog(buffer, extremedebug);
#endif

		
		if (bCW_) {
#ifdef DEBUG
			static const char* const buffer PROGMEM = "counterclockwise: ";
			logger_g_->LnWriteLog(buffer, extremedebug);
#endif

			turn_ = -1;
			if (oldTurn_ == turn_)
			{
				turn_ = 0;
			}
		}
		else {
#ifdef DEBUG
			static const char* const buffer PROGMEM = "clockwise: ";
			logger_g_->LnWriteLog(buffer, extremedebug);
#endif

			turn_ = 1;
			if (oldTurn_ == turn_)
			{
				turn_ = 0;
			}
		}
#ifdef DEBUG
		static const char* const buffer1 PROGMEM = "Encoder Position: ";
		logger_g_->LnWriteLog(buffer1, sensordata);
		logger_g_->WriteLog(encoderPosCount_, sensordata);
		static const char* const buffer2 PROGMEM = "Encoder return: ";
		logger_g_->LnWriteLog(buffer2, sensordata);
		logger_g_->WriteLog(turn_, sensordata);
#endif

	}
	else
	{
		turn_ = 0;
	}
	oldCLKVal_ = CLKVal_;
	oldTurn_ = turn_;
	return  turn_;
}

bool directionEncoder::getValue()
{
#ifdef DEBUG
	static const char* const buffer PROGMEM = "Call - EnSwitch";
	logger_g_->LnWriteLog(buffer, extremedebug);
#endif

	if (digitalRead(getPin()) == HIGH)
	{
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

bool directionEncoder::getSwitchValueDoggle()
{
#ifdef DEBUG
	static const char* const buffer PROGMEM = "Call - EnSwitchDoggle";
	logger_g_->LnWriteLog(buffer, extremedebug);
#endif

	if (digitalRead(getPin()))
	{
		bdoggleState_ = !bdoggleState_;
#ifdef DEBUG
		static const char* const buffer PROGMEM = "Doggled";
		logger_g_->LnWriteLog(buffer, sensordata);
#endif

	}
	return bdoggleState_;
}

bool directionEncoder::getSwitchLongValue()
{
#ifdef DEBUG
	static const char* const buffer PROGMEM = "Call - getSwitchLongValue";
	logger_g_->LnWriteLog(buffer, extremedebug);
#endif


	if (digitalRead(getPin()) == LOW && iSwitchState_ != 0)
	{
		iSwitchState_ = 0;
		iSwitchValue_ = true;
#ifdef DEBUG
		static const char* const buffer PROGMEM = "Switch is true";
		logger_g_->LnWriteLog(buffer, sensordata);
#endif
	}

	if (digitalRead(getPin()) == HIGH && iSwitchState_ != 1)
	{
		iSwitchState_ = 1;
		iSwitchValue_ = false;
#ifdef DEBUG
		static const char* const buffer PROGMEM = "Switch is false";
		logger_g_->LnWriteLog(buffer, sensordata);
#endif
	}
	return iSwitchValue_;
}