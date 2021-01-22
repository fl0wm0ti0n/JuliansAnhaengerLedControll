// @file           home_constants.cpp
// @author         flow@p0cki.at
// @date           12.2020
// @brief          define all constants

#ifndef _CONSTANTS_h
#define _CONSTANTS_h

// Run Test Yes/No
#define _TEST	1
#define _SWITCHTESTPERIOD	120000

// Debug Yes/No
//#define _DEBUG

// Meminfo Yes/No
//#define MEMINFO_

#ifdef _DEBUG
#define DEBUG_PRINT(x)     Serial.print (x)
#define DEBUG_PRINTLN(x)  Serial.println (x)
#else
#define DEBUG_PRINT(x)
#define DEBUG_PRINTLN(x)
#endif


// verwendete Hardware
// es darf immer nur eine Zeile true sein!
#define NANO		true
#define NodeMCUV3	false
#define UNO			false // noch nicht verwendet
#define MEGA		false // noch nicht verwendet

//****** PWM OUTPUT PINS ******
// On most Arduino boards(those with the ATmega168 or ATmega328), this function works on pins 3, 5, 6, 9, 10,
// and 11. On the Arduino Mega, it works on pins 2 - 13 and 44 - 46. Older Arduino boards with an
// ATmega8 only support analogWrite() on pins 9, 10, and 11. 
// PIN_PWM_1:	6 (Nano), 0 (nodeMCU V3)
// PIN_PWM_2:	9 (Nano), 2 (nodeMCU V3)
#if (NANO == true)
#define PIN_PWM_1		3
#define PIN_PWM_2		5
#define PIN_PWM_3		6
#define PIN_PWM_4		9
#define PIN_PWM_5		10
#define PIN_PWM_6		11
#endif
#if (NodeMCUV3 == true)
#define PIN_PWM_1		0
#define PIN_PWM_2		2
#endif


//****** INPUT PINS ******
#if (NANO == true)
#define	PIN_BUTTON_1	2
#define	PIN_BUTTON_2	4
#define	PIN_POTI_1		7
#endif

#if (NodeMCUV3 == true)
#define	PIN_HUM_1		14
#define	PIN_TOUCH_2		16
#define	PIN_MOTION_1	10
#define	PIN_MOTION_2	
#define PIN_SONIC_1		
#define PIN_SONIC_2
#define PIN_GAS_MQ135_1	A0
#define PIN_GAS_MQ7_1	
#define PIN_DUST_1		
#define PIN_ENCODER_SW	9
#define PIN_ENCODER_DT	13
#define PIN_ENCODER_CLK	12
#endif


//****** DEFAULT *********
#define	DEFAULT_LOGLEVEL		extremedebug
#define	DEFAULT_LOGTARGET		serial
#define	DEFAULT_LIGHTFDELAY		20
#define DEFAULT_THRESHOLD		0.5
#define DEFAULT_MAXVALUE		255
#define DEFAULT_MINVALUE		0
#define DEFAULT_ALLLEDS			-1
#define DEFAULT_BAUTRATE		9600


// Definitionen der einzelnen Schaltzeiten
#define		laufzeit_1		1UL
#define		laufzeit_5		5UL
#define		laufzeit_10		10UL
#define		laufzeit_15		15UL
#define		laufzeit_20		20UL
#define		laufzeit_25		25UL
#define     laufzeit_30		30UL
#define     laufzeit_50		50UL
#define     laufzeit_150	100UL
#define     laufzeit_100	150UL
#define     laufzeit_250	250UL
#define     laufzeit_500	500UL
#define     laufzeit_1k		1000UL
#define     laufzeit_1k5	1500UL
#define     laufzeit_2k		2000UL
#define     laufzeit_2k5	2500UL
#define     laufzeit_5k		5000UL


#define		BUTTON_ON_HOLDTIME			50UL	// nur einmal kurz drücken
#define		BUTTON_OFF_HOLDTIME			3000UL	// 3 Sekunden
#define		BUTTON_SATURATION_THRESHOLD	500UL	// Abstant zwischen drücken
#define		BUTTON_SATURATION_COUNT		2UL		// Abstant zwischen drücken

#endif