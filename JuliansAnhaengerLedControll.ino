// @file           *.ino
// @author         flow@p0cki.net
// @date           12.2020
// @brief          main routine

// Wenn als Board das NodeMCU V3 gewählt wurde wird dies entsprechend freigeschalten.

#include "constants.h"		// Constanten müssen vor allem anderen initialisiert werden

#include "logger.h"
//#include "PinChangeInt.h"
#include "RGBLed.h"
#include "digitalIn.h"
#include <EEPROM.h>

#include "MemoryInfo.h"
#include "statesController.h"
#include "SwitchController.h"
#include "Test_SwitchController.h"


//*******************************************************
//********************* DECLARATION *********************
//*******************************************************
// Objekte welche sofort benötigt werden.


// Variablen deklarieren in denen die Startzeiten
// der einzelnen Zeitfunktionen gespeichert werden
unsigned long zeitstempel1 = 0;
unsigned long zeitstempel2 = 0;
unsigned long zeitstempel3 = 0;

//volatile boolean g_volatSignal = false;
//volatile boolean g_volatSignalSet = false;
//volatile boolean g_volatSignalOld = false;
//volatile short interruptCount = 0;

//*******************************************************
//********************** Methoden ***********************
//*******************************************************


//void setSignal()
//{
//	volatSignal = digitalRead();
//	if (volatSignal != volatSignalOld)
//	{
//		if (volatSignal)
//		{
//			interruptCount++;
//			volatSignalSet = true;
//		}
//	}
//	volatSignalOld = volatSignal;
//}

//*******************************************************
//************************ SETUP ************************s
//*******************************************************
//char loggtitle[] PROGMEM = "Logging1";
logger* logger_global = logger::GetInstance(DEFAULT_LOGLEVEL, DEFAULT_LOGTARGET, "Logging1");
static char* const buffer2 PROGMEM = "RGBStripe1";
RGBLed rgbLed01(PIN_PWM_1, PIN_PWM_2, PIN_PWM_3, RGBLed::COMMON_ANODE, buffer2);
static char* const buffer3 PROGMEM = "RGBStripe2";
RGBLed rgbLed02(PIN_PWM_4, PIN_PWM_5, PIN_PWM_6, RGBLed::COMMON_ANODE, buffer3);

digitalIn rgbColorswitch1("rgbColorswitch1", PIN_BUTTON_1);
digitalIn rgbColorswitch2("rgbColorswitch2", PIN_BUTTON_2);
SwitchController rgbSwitchcontroller1(5000, 2000);
SwitchController rgbSwitchcontroller2(5000, 2000);

void setup()
{
	Serial.begin(9600);
	while (!Serial && millis() < 500); //Wait for Serial to be ready
	Serial.println(F("Serial Ready!"));
#ifdef DEBUG
	static const char* const buffer1 PROGMEM = "Setup Begin";
	logger_global->LnWriteLog(buffer1, debug);
#endif
	
#ifdef MEMINFO_
	free_mem();
	ram_detail();
#endif
	
#ifdef _TEST
	Test_SwitchController test;
	test.Run_Test();
#endif
	
	// Events 1:
	Serial.println(F("Setup Events for Button1"));
	static const char* const name1 PROGMEM = "on";
	rgbSwitchcontroller1.New_event(name1, 0, 1, 3000, 200, true, false);
	static const char* const name2 PROGMEM = "rgb_color";
	rgbSwitchcontroller1.New_event(name2, 0, 1, 500, 100, false, false);
	static const char* const name3 PROGMEM = "rgb_bright";
	rgbSwitchcontroller1.New_event(name3, 100, 2, 100, 100, false, false);
	static const char* const name4 PROGMEM = "off";
	rgbSwitchcontroller1.New_event(name4, 0, 1, 5000, 200, false, true);

	// Events 2:
	Serial.println(F("Setup Events for Button2"));
	static const char* const name11 PROGMEM = "on";
	rgbSwitchcontroller1.New_event(name11, 0, 1, 3000, 200, true, false);
	static const char* const name22 PROGMEM = "rgb_color";
	rgbSwitchcontroller1.New_event(name22, 0, 1, 500, 100, false, false);
	static const char* const name33 PROGMEM = "rgb_bright";
	rgbSwitchcontroller1.New_event(name33, 100, 2, 100, 100, false, false);
	static const char* const name44 PROGMEM = "off";
	rgbSwitchcontroller1.New_event(name44, 0, 1, 5000, 200, false, true);
	
#ifdef DEBUG
	static const char* const buffer4 PROGMEM = "Setup End";
	logger_global->LnWriteLog(buffer4, debug);
#endif

#ifdef MEMINFO_
	free_mem();
	ram_detail();
#endif

}

//*******************************************************
//************************ LOOP *************************
//*******************************************************
void loop()
{
	if (millis() - zeitstempel1 >= laufzeit_5)
	{
		auto result = rgbSwitchcontroller1.Event_handler(rgbColorswitch1.getValue(),millis());

		//Serial.println(millis());
		if (result && strcmp(rgbSwitchcontroller1.last_triggerede_event, "rgb_color") == 0)
		{
			Serial.println(F("->nextC"));
			rgbLed01.nextcolor();
		}
		if (result && strcmp(rgbSwitchcontroller1.last_triggerede_event, "rgb_bright") == 0)
		{
			Serial.println(F("->nextB"));
			rgbLed01.nextbrightness();
		}
		if (result && strcmp(rgbSwitchcontroller1.last_triggerede_event, "on") == 0)
		{
			Serial.println(F("->on"));
			rgbLed01.on(255, 255, 255, 255);
		}
		if (result && strcmp(rgbSwitchcontroller1.last_triggerede_event, "off") == 0)
		{
			Serial.println(F("->off"));
			rgbLed01.off();
		}
		auto result2 = rgbSwitchcontroller2.Event_handler(rgbColorswitch2.getValue(), millis());
		//Serial.println(millis());
		if (result2 && strcmp(rgbSwitchcontroller1.last_triggerede_event, "on") == 0)
		{
			Serial.println(F("->on2"));
			rgbLed01.on(255, 255, 255, 255);
		}
		if (result2 && strcmp(rgbSwitchcontroller1.last_triggerede_event, "off") == 0)
		{
			Serial.println(F("->off2"));
			rgbLed01.off();
		}
		if (result2 && strcmp(rgbSwitchcontroller1.last_triggerede_event, "rgb_color") == 0)
		{
			Serial.println(F("->nextC2"));
			rgbLed01.nextcolor();
		}
		if (result2 && strcmp(rgbSwitchcontroller1.last_triggerede_event, "rgb_bright") == 0)
		{
			Serial.println(F("->nextB2"));
			rgbLed01.nextbrightness();
		}
		zeitstempel1 = millis();
	}
}
