// @file           *.ino
// @author         flow@p0cki.net
// @date           12.2020
// @brief          main routine

// Wenn als Board das NodeMCU V3 gewählt wurde wird dies entsprechend freigeschalten.

#include <Bounce2.h>
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
bool flipflop1 = false;
bool result1 = false;
bool flipflop2 = false;
bool result2 = false;
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
	rgbSwitchcontroller1.New_event(name3, 200, 2, 200, 50, false, false);
	static const char* const name4 PROGMEM = "off";
	rgbSwitchcontroller1.New_event(name4, 0, 1, 5000, 200, false, true);

	// Events 2:
	Serial.println(F("Setup Events for Button2"));
	static const char* const name11 PROGMEM = "on";
	rgbSwitchcontroller2.New_event(name11, 0, 1, 3000, 200, true, false);
	static const char* const name22 PROGMEM = "rgb_color";
	rgbSwitchcontroller2.New_event(name22, 0, 1, 500, 100, false, false);
	static const char* const name33 PROGMEM = "rgb_bright";
	rgbSwitchcontroller2.New_event(name33, 200, 2, 200, 50, false, false);
	static const char* const name44 PROGMEM = "off";
	rgbSwitchcontroller2.New_event(name44, 0, 1, 5000, 200, false, true);
	
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
	rgbColorswitch1.update_debounce();
	rgbColorswitch2.update_debounce();
	
	if (millis() - zeitstempel1 >= laufzeit_5)
	{
		// mach nur bei Aenderung
		if (!flipflop1 && rgbColorswitch1.getValue())
		{
			Serial.println(F("-->true1"));
			result1 = rgbSwitchcontroller1.Event_handler(true, millis());
			flipflop1 = true;
		}
		else if (flipflop1 && !rgbColorswitch1.getValue())
		{
			Serial.println(F("-->false1"));
			result1 = rgbSwitchcontroller1.Event_handler(false, millis());
			flipflop1 = false;
		}
		
		// mach nur bei Aenderung
		if (!flipflop2 && rgbColorswitch2.getValue())
		{
			Serial.println(F("-->true2"));
			result2 = rgbSwitchcontroller2.Event_handler(true, millis());
			flipflop2 = true;
		}
		else if (flipflop2 && !rgbColorswitch2.getValue())
		{
			Serial.println(F("-->false2"));
			result2 = rgbSwitchcontroller2.Event_handler(false, millis());
			flipflop2 = false;
		}
		
		if (result1 && strcmp(rgbSwitchcontroller1.last_triggerede_event, "on") == 0)
		{
			Serial.println(F("->on"));
			rgbLed01.on(255, 255, 255, 255);
			result1 = false;
		}
		if (result1 && strcmp(rgbSwitchcontroller1.last_triggerede_event, "off") == 0)
		{
			Serial.println(F("->off"));
			rgbLed01.off();
			result1 = false;
		}
		if (result1 && strcmp(rgbSwitchcontroller1.last_triggerede_event, "rgb_color") == 0)
		{
			Serial.println(F("->nextC"));
			rgbLed01.nextcolor();
			result1 = false;
		}
		if (result1 && strcmp(rgbSwitchcontroller1.last_triggerede_event, "rgb_bright") == 0)
		{
			Serial.println(F("->nextB"));
			rgbLed01.nextbrightness();
			result1 = false;
		}

		if (result2 && strcmp(rgbSwitchcontroller2.last_triggerede_event, "on") == 0)
		{
			Serial.println(F("->on2"));
			rgbLed02.on(255, 255, 255, 255);
			result2 = false;
		}
		if (result2 && strcmp(rgbSwitchcontroller2.last_triggerede_event, "off") == 0)
		{
			Serial.println(F("->off2"));
			rgbLed02.off();
			result2 = false;
		}
		if (result2 && strcmp(rgbSwitchcontroller2.last_triggerede_event, "rgb_color") == 0)
		{
			Serial.println(F("->nextC2"));
			rgbLed02.nextcolor();
			result2 = false;
		}
		if (result2 && strcmp(rgbSwitchcontroller2.last_triggerede_event, "rgb_bright") == 0)
		{
			Serial.println(F("->nextB2"));
			rgbLed02.nextbrightness();
			result2 = false;
		}
		zeitstempel1 = millis();
	}
}
