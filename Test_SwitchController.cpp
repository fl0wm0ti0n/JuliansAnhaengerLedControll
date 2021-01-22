#include "Test_SwitchController.h"
#include "MemoryInfo.h"

Test_SwitchController::Test_SwitchController()
{
	logger_g_ = logger::GetInstance();
	new_switch_controller = SwitchController(10000, 5000);

#ifdef _DEBUG
	logger_g_->LnWriteLog(F("Call - Test_SwitchController"), extremedebug);
#endif

	Setup();
}

Test_SwitchController::~Test_SwitchController()
= default;

void Test_SwitchController::Setup()
{
#ifdef _DEBUG
	logger_g_->LnWriteLog(F("Call - Setup"), extremedebug);
#endif
#ifdef MEMINFO_
	free_mem();
#endif
	
	trigger1_ = F("PowerOn");
	trigger2_ = F("1Click");
	trigger3_ = F("2Clicks");
	trigger4_ = F("PowerOff");

	new_switch_controller.New_event(trigger1_, 0, 1, 5000, 1000, true, false);
#ifdef MEMINFO_
	ram_detail();
#endif
	new_switch_controller.New_event(trigger2_, 0, 1, 3000, 1000, false, false);
#ifdef MEMINFO_
	ram_detail();
#endif
	new_switch_controller.New_event(trigger3_, 2000, 2, 2000, 1000, false, false);
#ifdef MEMINFO_
	ram_detail();
#endif
	new_switch_controller.New_event(trigger4_, 0, 1, 10000, 2000, false, true);
#ifdef MEMINFO_
	ram_detail();
#endif

}

void Test_SwitchController::Run_Test()
{
#ifdef _DEBUG
	logger_g_->LnWriteLog(F("Run_Test"), extremedebug);
#endif
#ifdef MEMINFO_
	free_mem();
#endif

	String trennlinie_ = F("------------------------------");
	bool done_check[7] = { false, false, false, false, false, false, false };
	bool result = false;
	unsigned long startzeit = millis();
	unsigned long zeitstempel = startzeit;
	
	logger_g_->LnWriteLog(trennlinie_, debug);
	Zwischenzeit(zeitstempel);
	Zeitstempel(zeitstempel);
	if (Simulate_FirstClick())
	{
		logger_g_->LnWriteLog(F("TEST SUCCESS"), debug);
	}
	else
	{
		logger_g_->LnWriteLog(F("TEST FAILED"), debug);
	}
	
	zeitstempel = millis();
	while (millis() <= testtime_)
	{
		if (millis() - zeitstempel >= new_switch_controller.time_between_trigger && !done_check[0])
		{
			logger_g_->LnWriteLog(trennlinie_, debug);
			Zwischenzeit(zeitstempel);
			Zeitstempel(zeitstempel);
			if (Simulate_1Click(startzeit))
			{
				logger_g_->LnWriteLog(F("TEST SUCCESS"), debug);
			}
			else
			{
				logger_g_->LnWriteLog(F("TEST FAILED"), debug);
			}
			zeitstempel = millis();
			done_check[0] = true;
		}
		if (millis() - zeitstempel >= new_switch_controller.time_between_trigger && !done_check[1] && done_check[0])
		{
			logger_g_->LnWriteLog(trennlinie_, debug);
			Zwischenzeit(zeitstempel);
			Zeitstempel(zeitstempel);
			if (Simulate_2Clicks(startzeit))
			{
				logger_g_->LnWriteLog(F("TEST SUCCESS"), debug);
			}
			else
			{
				logger_g_->LnWriteLog(F("TEST FAILED"), debug);
			}
			zeitstempel = millis();
			done_check[1] = true;
		}

		if (millis() - zeitstempel >= new_switch_controller.time_between_trigger && !done_check[2] && done_check[1])
		{
			logger_g_->LnWriteLog(trennlinie_, debug);
			Zwischenzeit(zeitstempel);
			Zeitstempel(zeitstempel);
			if (Simulate_Timeout(startzeit))
			{
				logger_g_->LnWriteLog(F("TEST SUCCESS"), debug);
			}
			else
			{
				logger_g_->LnWriteLog(F("TEST FAILED"), debug);
			}
			zeitstempel = millis();
			done_check[2] = true;
		}
		if (millis() - zeitstempel >= new_switch_controller.time_between_trigger && !done_check[3] && done_check[2])
		{
			logger_g_->LnWriteLog(trennlinie_, debug);

			if (Simulate_TimeBetweenTrigger(startzeit))
			{
				logger_g_->LnWriteLog(F("TEST SUCCESS"), debug);
			}
			else
			{
				logger_g_->LnWriteLog(F("TEST FAILED"), debug);
			}
			zeitstempel = millis();
			done_check[3] = true;
		}
		if (millis() - zeitstempel >= new_switch_controller.time_between_trigger && !done_check[4] && done_check[3])
		{
			logger_g_->LnWriteLog(trennlinie_, debug);
			Zwischenzeit(zeitstempel);
			Zeitstempel(zeitstempel);
			if (Simulate_WrongInput1(startzeit))
			{
				logger_g_->LnWriteLog(F("TEST SUCCESS"), debug);
			}
			else
			{
				logger_g_->LnWriteLog(F("TEST FAILED"), debug);
			}
			zeitstempel = millis();
			done_check[4] = true;
		}
		if (millis() - zeitstempel >= new_switch_controller.time_between_trigger && !done_check[5] && done_check[4])
		{
			logger_g_->LnWriteLog(trennlinie_, debug);
			Zwischenzeit(zeitstempel);
			Zeitstempel(zeitstempel);
			if (Simulate_WrongInput2(startzeit))
			{
				logger_g_->LnWriteLog(F("TEST SUCCESS"), debug);
			}
			else
			{
				logger_g_->LnWriteLog(F("TEST FAILED"), debug);
			}
			zeitstempel = millis();
			done_check[5] = true;
		}
		if (millis() - zeitstempel >= new_switch_controller.time_between_trigger && !done_check[6] && done_check[5])
		{
			logger_g_->LnWriteLog(trennlinie_, debug);
			Zwischenzeit(zeitstempel);
			Zeitstempel(zeitstempel);
			if (Simulate_LastClick(startzeit))
			{
				logger_g_->LnWriteLog(F("TEST SUCCESS"), debug);
			}
			else
			{
				logger_g_->LnWriteLog(F("TEST FAILED"), debug);
			}
			zeitstempel = millis();
			done_check[6] = true;
		}
	}
}

bool Test_SwitchController::Validate(bool vali, String name_of_trigger)
{
#ifdef _DEBUG
	logger_g_->LnWriteLog(F("Call - Validate"), extremedebug);
#endif
#ifdef MEMINFO_
	free_mem();
#endif
	
	if (vali)
	{
		for (int idx = 0; idx < new_switch_controller.events.size(); ++idx)
		{
			if (new_switch_controller.events[idx].prop_event_title == name_of_trigger && new_switch_controller.events[idx].vali_is_triggered_)
			{
				vali = true;
				logger_g_->LnWriteLog(String(new_switch_controller.events[idx].prop_event_title), debug);
				logger_g_->WriteLog(F(" wurde ausgeloest"), debug);
			}

			if (new_switch_controller.events[idx].prop_event_title != name_of_trigger && new_switch_controller.events[idx].vali_is_triggered_)
			{
				vali = false;
				logger_g_->LnWriteLog(String(new_switch_controller.events[idx].prop_event_title), debug);
				logger_g_->WriteLog(F(" wurde ausgeloest"), debug);
			}
		}
		if (!vali)
		{
			logger_g_->LnWriteLog(F(" Es wurden mehrere Events ausgeloest"), debug);
		}
	}
	else
	{
		vali = false;
		logger_g_->LnWriteLog(F("Es wurde kein Event ausgeloest"), debug);
	}

	return vali;
}

bool Test_SwitchController::ValidateNegative(bool vali, String name_of_trigger)
{
#ifdef _DEBUG
	logger_g_->LnWriteLog(F("Call - ValidateNegative"), extremedebug);
#endif
#ifdef MEMINFO_
	free_mem();
#endif
	
	if (vali)
	{
		for (int idx = 0; idx < new_switch_controller.events.size(); ++idx)
		{
			if (new_switch_controller.events[idx].vali_is_triggered_)
			{
				vali = false;
				logger_g_->LnWriteLog(String(new_switch_controller.events[idx].prop_event_title), debug);
				logger_g_->WriteLog(F(" wurde fälschlich ausgeloest"), debug);
			}
		}
	}
	else
	{
		vali = true;
		logger_g_->LnWriteLog(F("Es wurde kein Event ausgeloest"), debug);
	}

	return vali;
}

bool Test_SwitchController::ValidateTimeout(bool vali)
{
#ifdef _DEBUG
	logger_g_->LnWriteLog(F("Call - ValidateTimeout"), extremedebug);
#endif
#ifdef MEMINFO_
	free_mem();
#endif
	
	if (vali)
	{
		for (int idx = 0; idx < new_switch_controller.events.size(); ++idx)
		{
			if (new_switch_controller.events[idx].vali_is_triggered_ = false)
			{
				vali = true;
				logger_g_->LnWriteLog(F("vali_is_triggered_ ist resettiert"), debug);
			}
			else
			{
				vali = false;
			}

			if (new_switch_controller.events[idx].vali_triggered_time = 0)
			{
				vali = true;
				logger_g_->LnWriteLog(F("vali_triggered_time ist resettiert"), debug);
			}
			else
			{
				vali = false;
			}

			if (new_switch_controller.events[idx].vali_is_failed_ = false)
			{
				vali = true;
				logger_g_->LnWriteLog(F("vali_is_failed_ ist resettiert"), debug);
			}
			else
			{
				vali = false;
			}
		}
	}
	return vali;
}

bool Test_SwitchController::DoIfTimeForButtonRelease(byte idx, short tolerance, unsigned long zeitstempel, bool result)
{
#ifdef _DEBUG
	logger_g_->LnWriteLog(F("Call - DoIfTimeForButtonRelease"), extremedebug);
#endif
#ifdef MEMINFO_
	free_mem();
#endif
	logger_g_->LnWriteLog(F("prop_continuous_hold: "), debug);
	logger_g_->WriteLog(String(new_switch_controller.events[idx].prop_continuous_hold), debug);
	do {
		Zwischenzeit(zeitstempel);
		if (millis() - zeitstempel >= new_switch_controller.events[idx].prop_continuous_hold)
		{
			Zwischenzeit(zeitstempel);
			logger_g_->LnWriteLog(F("Release Button"), debug);
			result = new_switch_controller.Event_handler(false, millis());
			zeitstempel = millis();
			break;
		}
	} while (millis() - zeitstempel < new_switch_controller.events[idx].prop_continuous_hold + tolerance);
	return result;
}

bool Test_SwitchController::DoIfTimeForButtonPress(byte idx, short tolerance, unsigned long zeitstempel, bool result)
{
#ifdef _DEBUG
	logger_g_->LnWriteLog(F("Call - DoIfTimeForButtonPress"), extremedebug);
#endif
#ifdef MEMINFO_
	free_mem();
#endif
	logger_g_->LnWriteLog(F("prop_threshhold_between: "), debug);
	logger_g_->WriteLog(String(new_switch_controller.events[idx].prop_threshhold_between), debug);
	do {
		Zwischenzeit(zeitstempel);
		if (millis() - zeitstempel >= new_switch_controller.events[idx].prop_threshhold_between)
		{
			Zwischenzeit(zeitstempel);
			logger_g_->LnWriteLog(F("Press Button"), debug);
			result = new_switch_controller.Event_handler(true, millis());
			zeitstempel = millis();
			break;
		}
	} while (millis() - zeitstempel < new_switch_controller.events[idx].prop_threshhold_between + tolerance);
	return result;
}

bool Test_SwitchController::Simulate_FirstClick()
{
#ifdef _DEBUG
	logger_g_->LnWriteLog(F("Call - Simulate_FirstClick"), extremedebug);
#endif
#ifdef MEMINFO_
	free_mem();
#endif
	bool result = false;
	unsigned long zeitstempel = millis();
	
	logger_g_->LnWriteLog(String(trigger1_), debug);
	logger_g_->WriteLog(F(" wird getestet"), debug);

	if (!new_switch_controller.is_on)
	{
		logger_g_->LnWriteLog(F("Press Button"), debug);
		
		result = new_switch_controller.Event_handler(true, millis());
		zeitstempel = millis();
		result = DoIfTimeForButtonRelease(0, 500, zeitstempel, result);
	}
	else
	{
		logger_g_->LnWriteLog(String(trigger1_), debug);
		logger_g_->WriteLog(F("Testerror, Zustand ist schon 'On' "), debug);
	}

	return Validate(result, trigger1_);
}

bool Test_SwitchController::Simulate_1Click(unsigned long startzeit)
{
#ifdef _DEBUG
	logger_g_->LnWriteLog(F("Call - Simulate_1Click"), extremedebug);
#endif
#ifdef MEMINFO_
	free_mem();
#endif
	bool result = false;
	unsigned long zeitstempel = startzeit;

	logger_g_->LnWriteLog(String(trigger2_), debug);
	logger_g_->WriteLog(F(" wird getestet"), debug);

	if (new_switch_controller.is_on)
	{
		logger_g_->LnWriteLog(F("Press Button"), debug);
		
		result = new_switch_controller.Event_handler(true, millis());
		zeitstempel = millis();

		result = DoIfTimeForButtonRelease(1, 500, zeitstempel, result);
	}
	else
	{
		logger_g_->LnWriteLog(String(trigger2_), debug);
		logger_g_->WriteLog(F(" Testerror, Zustand ist 'Off'"), debug);
	}

	return Validate(result, trigger2_);
}

bool Test_SwitchController::Simulate_2Clicks(unsigned long startzeit)
{
#ifdef _DEBUG
	logger_g_->LnWriteLog(F("Call - Simulate_2Clicks"), extremedebug);
#endif
#ifdef MEMINFO_
	free_mem();
#endif
	bool result = false;
	unsigned long zeitstempel = millis();

	logger_g_->LnWriteLog(String(trigger3_), debug);
	logger_g_->WriteLog(F(" wird getestet"), debug);
	
	if (new_switch_controller.is_on)
	{
		logger_g_->LnWriteLog(F("Press Button"), debug);
		
		result = new_switch_controller.Event_handler(true, millis());
		zeitstempel = millis();

		result = DoIfTimeForButtonRelease(2, 500, zeitstempel, result);
		result = DoIfTimeForButtonPress(2, 500, zeitstempel, result);
		result = DoIfTimeForButtonRelease(2, 500, zeitstempel, result);

	}
	else
	{
		logger_g_->LnWriteLog(String(trigger3_), debug);
		logger_g_->WriteLog(F(" Testerror, Zustand ist 'Off'"), debug);
	}

	return Validate(result, trigger3_);
}

bool Test_SwitchController::Simulate_LastClick(unsigned long startzeit)
{
#ifdef _DEBUG
	logger_g_->LnWriteLog(F("Call - Simulate_LastClick"), extremedebug);
#endif
#ifdef MEMINFO_
	free_mem();
#endif
	
	bool result = false;
	unsigned long zeitstempel = millis();

	logger_g_->LnWriteLog(String(trigger4_), debug);
	logger_g_->WriteLog(F(" wird getestet"), debug);
	
	if (new_switch_controller.is_on)
	{
		logger_g_->LnWriteLog(F("Press Button"), debug);
		
		result = new_switch_controller.Event_handler(true, millis());
		zeitstempel = millis();

		result = DoIfTimeForButtonRelease(3, 500, zeitstempel, result);
	}
	else
	{
		logger_g_->LnWriteLog(String(trigger4_), debug);
		logger_g_->WriteLog(F(" Testerror, Zustand ist 'Off' "), debug);
	}

	return Validate(result, trigger4_);
}

bool Test_SwitchController::Simulate_Timeout(unsigned long startzeit)
{
#ifdef _DEBUG
	logger_g_->LnWriteLog(F("Call - Simulate_Timeout"), extremedebug);
#endif
#ifdef MEMINFO_
	logger_g_->WriteLog(F(", Freemem: "), debug);
	logger_g_->WriteLog(String(free_memory()), debug);
#endif
	
	bool result = false;
	unsigned long zeitstempel = millis();

	logger_g_->LnWriteLog(F("Timeout wird getestet ... millis: "), debug);
	logger_g_->WriteLog(String(millis()), debug);
	
	if (new_switch_controller.timeout)
	{
		do {
			if (millis() - zeitstempel >= new_switch_controller.timeout)
			{
				logger_g_->LnWriteLog(F("Timeout erreicht ... millis: "), debug);
				logger_g_->WriteLog(String(millis()), debug);
				
				return ValidateTimeout(result);
			}
		} while (millis() - zeitstempel <= new_switch_controller.timeout);
	}
}

bool Test_SwitchController::Simulate_WrongInput1(unsigned long startzeit)
{
#ifdef _DEBUG
	logger_g_->LnWriteLog(F("Call - Simulate_WrongInput1"), extremedebug);
#endif
#ifdef MEMINFO_
	free_mem();
#endif
	
	bool result = false;
	unsigned long zeitstempel = millis();

	logger_g_->LnWriteLog(F("Fehlerhafte Eingabe Test (zu lange gedrueckt)"), debug);
	
	if (new_switch_controller.is_on)
	{
		logger_g_->LnWriteLog(F("Press Button"), debug);
		
		result = new_switch_controller.Event_handler(true, millis());
		zeitstempel = millis();


		result = DoIfTimeForButtonRelease(1, 500, zeitstempel, result);
	}
	else
	{
		logger_g_->LnWriteLog(String(trigger2_), debug);
		logger_g_->WriteLog(F(" Testerror, Zustand ist 'Off'"), debug);
		return false;
	}

	return ValidateNegative(result, trigger2_);
}

bool Test_SwitchController::Simulate_WrongInput2(unsigned long startzeit)
{
#ifdef _DEBUG
	logger_g_->LnWriteLog(F("Call - Simulate_WrongInput2"), extremedebug);
#endif
#ifdef MEMINFO_
	free_mem();
#endif
	
	bool result = false;
	unsigned long zeitstempel = millis();

	logger_g_->LnWriteLog(F("Fehlerhafte eingabe Test (zu lange gedrueckt)"), debug);

	if (new_switch_controller.is_on)
	{
		logger_g_->LnWriteLog(F("Press Button"), debug);
		
		result = new_switch_controller.Event_handler(true, millis());
		zeitstempel = millis();

		result = DoIfTimeForButtonRelease(2, 500, zeitstempel, result);
		result = DoIfTimeForButtonPress(2, 500, zeitstempel, result);
		result = DoIfTimeForButtonRelease(2, 500, zeitstempel, result);
	}
	else
	{
		logger_g_->LnWriteLog(String(trigger3_), debug);
		logger_g_->WriteLog(F(" Testerror, Zustand ist 'Off'"), debug);
		return false;
	}

	return ValidateNegative(result, trigger3_);
}

bool Test_SwitchController::Simulate_TimeBetweenTrigger(unsigned long startzeit)
{
#ifdef _DEBUG
	logger_g_->LnWriteLog(F("Call - Simulate_TimeBetweenTrigger"), extremedebug);
#endif
#ifdef MEMINFO_
	free_mem();
#endif
	bool result = false;
	unsigned long zeitstempel = millis();

	logger_g_->LnWriteLog(F("Timeout Test... millis: "), debug);
	logger_g_->WriteLog(String(millis()), debug);
	
	if (new_switch_controller.timeout)
	{
		do {
			if (millis() - zeitstempel >= new_switch_controller.timeout)
			{
				logger_g_->LnWriteLog(F("Timeout erreicht ... millis: "), debug);
				logger_g_->WriteLog(String(millis()), debug);
				return ValidateTimeout(result);
			}
		} while (millis() - zeitstempel <= new_switch_controller.timeout);
	}
}

void Test_SwitchController::Zwischenzeit(unsigned long zeitstempel)
{
	logger_g_->LnWriteLog(F("Zwischenzeit: "), debug);
	logger_g_->WriteLog(String(millis() - zeitstempel), debug);
}
;
void Test_SwitchController::Zeitstempel(unsigned long zeitstempel)
{
	logger_g_->LnWriteLog(F("Zeitstempel: "), debug);
	logger_g_->WriteLog(String(zeitstempel), debug);
}
