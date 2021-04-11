
#ifndef _TEST_SWITCHCONTROLLER_h
#define _TEST_SWITCHCONTROLLER_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include "SwitchController.h"
#include "logger.h"

class Test_SwitchController
{
public:
	Test_SwitchController();
	~Test_SwitchController();
	void Run_Test();

private:
	void Setup();
	bool Validate(bool vali, const char name_of_trigger[]);
	bool ValidateNegative(bool vali, const char name_of_trigger[]);
	bool ValidateTimeout(bool vali);
	bool DoIfTimeForButtonRelease(unsigned short idx, short additive, unsigned long zeitstempel, bool result);
	bool DoIfTimeForButtonPress(unsigned short idx, short additive, unsigned long zeitstempel, bool result);

	bool Simulate_FirstClick();
	bool Simulate_SelectedNumberOfClicks(unsigned int eventtype, unsigned int clickcount, unsigned long startzeit);
	//bool Simulate_Timeout(unsigned long startzeit);
	bool Simulate_WrongInput1(unsigned long startzeit);
	bool Simulate_WrongInput2(unsigned long startzeit);
	//bool Simulate_TimeBetweenTrigger(unsigned long startzeit);
	bool Simulate_LastClick(unsigned long startzeit);

	void Zwischenzeit(unsigned long zeitstempel);
	void Zeitstempel(unsigned long zeitstempel);
	
	const unsigned long testtime_ = _SWITCHTESTPERIOD; // milliseconds
	
	const char* const trigger_1 PROGMEM = "PowerOn";
	const char* const trigger_2 PROGMEM = "1Click";
	const char* const trigger_3 PROGMEM = "2Clicks";
	const char* const trigger_4 PROGMEM = "PowerOff";

	// Initialisiere die Tabelle von chars
	const char* const trigger_array_[4] PROGMEM = { trigger_1, trigger_2, trigger_3, trigger_4 };

	logger* logger_g_;
	SwitchController new_switch_controller;

	EventTrigger new_trigger;
};

#endif