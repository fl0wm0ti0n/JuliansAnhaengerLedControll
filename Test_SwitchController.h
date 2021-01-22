
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
	bool Validate(bool vali, String name_of_trigger);
	bool ValidateNegative(bool vali, String name_of_trigger);
	bool ValidateTimeout(bool vali);
	bool DoIfTimeForButtonRelease(byte idx, short tolerance, unsigned long zeitstempel, bool result);
	bool DoIfTimeForButtonPress(byte idx, short tolerance, unsigned long zeitstempel, bool result);

	bool Simulate_FirstClick();
	bool Simulate_1Click(unsigned long startzeit);
	bool Simulate_2Clicks(unsigned long startzeit);
	bool Simulate_2ClicksForRow(unsigned long startzeit);
	bool Simulate_Timeout(unsigned long startzeit);
	bool Simulate_WrongInput1(unsigned long startzeit);
	bool Simulate_WrongInput2(unsigned long startzeit);
	bool Simulate_TimeBetweenTrigger(unsigned long startzeit);
	bool Simulate_LastClick(unsigned long startzeit);

	void Zwischenzeit(unsigned long zeitstempel);
	void Zeitstempel(unsigned long zeitstempel);
	
	const unsigned long testtime_ = _SWITCHTESTPERIOD; // milliseconds
		
	String trigger1_;
	String trigger2_;
	String trigger3_;
	String trigger4_;

	logger* logger_g_;
	SwitchController new_switch_controller;

	EventTrigger new_trigger;
};

#endif