#include "directionEncoder.h"

directionEncoder::directionEncoder(String n, int pSW, int pCLK, int pDT)
	:sensor(digitalIn_sens, n, pSW)
{
	logger_g_ = logger::GetInstance(DEFAULT_LOGLEVEL, DEFAULT_LOGTARGET, "Logging1");
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

	logger_g_->WriteLog(F("Call - GetEncoder"), extremedebug);
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
		logger_g_->WriteLog(F("Rotated "), extremedebug);
		
		if (bCW_) {
			logger_g_->WriteLog(F("counterclockwise: "), extremedebug);
			turn_ = -1;
			if (oldTurn_ == turn_)
			{
				turn_ = 0;
			}
		}
		else {
			logger_g_->WriteLog(F("clockwise: "), extremedebug);
			turn_ = 1;
			if (oldTurn_ == turn_)
			{
				turn_ = 0;
			}
		}
		logger_g_->WriteLog("Encoder Position: " + encoderPosCount_, sensordata);
		logger_g_->WriteLog("Encoder return: " + turn_, sensordata);
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
	logger_g_->WriteLog(F("Call - EnSwitch"), extremedebug);
	if (digitalRead(getPin()) == HIGH)
	{
		logger_g_->WriteLog(F("Switch: is true"), sensordata);
		bSwitchState_ = true;
	}
	else
	{
		logger_g_->WriteLog(F("Switch: is false"), sensordata);
		bSwitchState_ = false;
	}
	return bSwitchState_;
}

bool directionEncoder::getSwitchValueDoggle()
{
	logger_g_->WriteLog(F("Call - EnSwitchDoggle"), extremedebug);
	if (digitalRead(getPin()))
	{
		bdoggleState_ = !bdoggleState_;
		logger_g_->WriteLog(F("Doggled"), sensordata);
	}
	return bdoggleState_;
}

bool directionEncoder::getSwitchLongValue()
{
	logger_g_->WriteLog(F("Call - getSwitchLongValue"), extremedebug);

	if (digitalRead(getPin()) == LOW && iSwitchState_ != 0)
	{
		iSwitchState_ = 0;
		iSwitchValue_ = true;
		logger_g_->WriteLog(F("Switch is true"), sensordata);
	}

	if (digitalRead(getPin()) == HIGH && iSwitchState_ != 1)
	{
		iSwitchState_ = 1;
		iSwitchValue_ = false;
		logger_g_->WriteLog(F("Switch is false"), sensordata);
	}
	return iSwitchValue_;
}