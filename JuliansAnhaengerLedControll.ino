// @file           *.ino
// @author         flow@p0cki.net
// @date           12.2020
// @brief          main routine

// Wenn als Board das NodeMCU V3 gewählt wurde wird dies entsprechend freigeschalten.

#include "constants.h"		// Constanten müssen vor allem anderen initialisiert werden

#include "logger.h"
//#include "PinChangeInt.h"
#include "RGBLed.h"
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
unsigned long g_startzeit_1 = 0;

volatile boolean g_volatSignal = false;
volatile boolean g_volatSignalSet = false;
volatile boolean g_volatSignalOld = false;
volatile short interruptCount = 0;


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
//************************ SETUP ************************
//*******************************************************
void setup()
{
	logger* logger_global = logger::GetInstance(DEFAULT_LOGLEVEL, DEFAULT_LOGTARGET, F("Logging1"));
	Serial.begin(9600);
	while (!Serial && millis() < 500); //Wait for Serial to be ready

	logger_global->LnWriteLog(F("Setup Begin"), debug);
#ifdef MEMINFO_
	logger_global->WriteLog(F(", Freemem: "), debug);
	logger_global->WriteLog(String(free_memory()), debug);
	ram_detail();
#endif

	
#ifdef _TEST
	Test_SwitchController test;
	test.Run_Test();

	RGBLed rgbLed01(PIN_PWM_1, PIN_PWM_2, PIN_PWM_3, RGBLed::COMMON_ANODE, F("RGBStripe1"));
	RGBLed rgbLed02(PIN_PWM_4, PIN_PWM_5, PIN_PWM_6, RGBLed::COMMON_ANODE, F("RGBStripe1"));
#else

	//******************** Interrupts ***********************
	/*
	attachInterrupt
	LOW Interrupt wird getriggert, wenn der Pin LOW ist,
	CHANGE Interrupt wird getriggert, wenn der Pin den Wert ändert
	RISING Interrupt wird getriggert, wenn der Pin von LOW auf HIGH wechselt,
	FALLING Interrupt wird getriggert, wenn der Pin von HIGH auf LOW wechselt.
	Die Due - , Zero - und MKR1000 - Boards erlauben zusätzlich :
	HIGH Interrupt wird getriggert, wenn der Pin HIGH ist.*/

	//attachInterrupt(digitalPinToInterrupt(PIN_BUTTON_1), setSignal, CHANGE); // pressed
	//attachInterrupt(digitalPinToInterrupt(PIN_BUTTON_2), setSignal, CHANGE); // pressed
	//attachInterrupt(digitalPinToInterrupt(PIN_POTI_1), setSignal, CHANGE); // pressed
#endif

	logger_global->LnWriteLog(F("Setup End"), debug);
#ifdef MEMINFO_
	logger_global->WriteLog(F(", Freemem: "), debug);
	logger_global->WriteLog(String(free_memory()), debug);
	ram_detail();
#endif

}

//*******************************************************
//************************ LOOP *************************
//*******************************************************
void loop()
{

}