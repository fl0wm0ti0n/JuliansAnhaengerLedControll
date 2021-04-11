// @file           analogOut.h
// @author         flow@p0cki.net
// @date           08.2019
// @brief          class for analog output

/*
Writes an analog value(PWM wave) to a pin.Can be used to light a LED at varying brightnesses or 
drive a motor at various speeds.After a call to analogWrite(), the pin will generate a steady 
square wave of the specified duty cycle until the next call to analogWrite() (or a call to 
digitalRead() or digitalWrite() on the same pin).The frequency of the PWM signal on most pins 
is approximately 490 Hz.On the Uno and similar boards, pins 5 and 6 have a frequency of approximately 980 Hz.
Pins 3 and 11 on the Leonardo also Run at 980 Hz.

On most Arduino boards(those with the ATmega168 or ATmega328), this function works on pins 3, 5, 6, 9, 10, 
and 11. On the Arduino Mega, it works on pins 2 - 13 and 44 - 46. Older Arduino boards with an 
ATmega8 only support analogWrite() on pins 9, 10, and 11.
*/

#ifndef _ANALOGOUT_h
#define _ANALOGOUT_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include "actor.h"
#include "logger.h"

class analogOut :public actor
{
	int lightcounter_ = 0;
	logger* logger_g_;

public:
	analogOut(char n[], int p);
	virtual ~analogOut();
	virtual bool setValue(int v);
	bool doggle();
	void SlowlyIncreaseOrDecreaseValue(bool sensorResult, int maxValue);
};
#endif