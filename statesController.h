#pragma once
#ifndef _STATESCONTROLLER_h
#define _STATESCONTROLLER_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <EEPROM.h>

unsigned short eepromSize = EEPROM.length() - 1;

byte rgbStripeHueOne[3] = { 0,0,0 };
byte rgbStripeHueTwo[3] = { 0,0,0 };
byte rgbStripeStates[6] = { rgbStripeHueOne[1], rgbStripeHueOne[1], rgbStripeHueOne[1], rgbStripeHueTwo[1], rgbStripeHueTwo[2], rgbStripeHueTwo[3] };

enum StateAdresses
{
	Rgb1REDAdress = 1,
	Rgb1GREENAdress = 2,
	Rgb1BLUEAdress = 3,
	Rgb2REDAdress = 4,
	Rgb2GREENAdress = 5,
	Rgb2BLUEAdress = 6,
};

struct RgbChannel
{
	byte rgb1[3] = { PIN_PWM_1, PIN_PWM_2, PIN_PWM_3 };
	byte rgb2[3] = { PIN_PWM_4, PIN_PWM_5, PIN_PWM_6 };
};

template<typename stateType>
void saveState(const int adress, stateType value)
{
	EEPROM.write(adress, value);
}

template<typename stateType>
stateType readState(const int adress)
{
	stateType value = EEPROM.read(adress);
	return value;
}

void getLastStates()
{
	for (int StatesInt = Rgb1REDAdress; StatesInt != Rgb2BLUEAdress; StatesInt++)
	{
		auto temp = static_cast<StateAdresses>(StatesInt);
		rgbStripeStates[StatesInt - 1] = readState<int>(temp);
	}
}

void setRGBLastState(RGBLed obj1, RGBLed obj2)
{
	getLastStates();
	obj1.setColor(rgbStripeStates[1], rgbStripeStates[2], rgbStripeStates[3]);
	obj2.setColor(rgbStripeStates[4], rgbStripeStates[5], rgbStripeStates[6]);

	//TODO
}


#endif