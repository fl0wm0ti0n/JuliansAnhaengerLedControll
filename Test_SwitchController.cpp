#include "Test_SwitchController.h"
#include "MemoryInfo.h"

Test_SwitchController::Test_SwitchController()
{
#ifdef DEBUG
	logger_g_ = logger::GetInstance();
	static const char* const buffer PROGMEM = "Call - Test_SwitchController";
	logger_g_->LnWriteLog(buffer, extremedebug);
#endif

	new_switch_controller = SwitchController(10000, 5000);

	Setup();
}

Test_SwitchController::~Test_SwitchController()
= default;

void Test_SwitchController::Setup()
{
#ifdef DEBUG
	static const char* const buffer PROGMEM = "Call - Setup";
	logger_g_->LnWriteLog(buffer, extremedebug);
#endif
#ifdef MEMINFO_
	free_mem();
#endif

	new_switch_controller.New_event(trigger_array_[0], 0, 1, 5000, 500, true, false);
#ifdef MEMINFO_
	//ram_detail();
	free_mem();
#endif
	new_switch_controller.New_event(trigger_array_[1], 0, 1, 3000, 500, false, false);
#ifdef MEMINFO_
	//ram_detail();
	free_mem();
#endif
	new_switch_controller.New_event(trigger_array_[2], 2000, 2, 2000, 900, false, false);
#ifdef MEMINFO_
	//ram_detail();
	free_mem();
#endif
	new_switch_controller.New_event(trigger_array_[3], 0, 1, 10000, 500, false, true);
#ifdef MEMINFO_
	//ram_detail();
	free_mem();
#endif

}

void Test_SwitchController::Run_Test()
{
#ifdef DEBUG
	static const char* const buffer PROGMEM = "Run_Test";
	logger_g_->LnWriteLog(buffer, extremedebug);
#endif
#ifdef MEMINFO_
	free_mem();
#endif

	static const char* const trennlinie_ PROGMEM = "------------------------------";
	static const char* const buffer_succ PROGMEM = "TEST SUCCESS";
	static const char* const buffer_fail PROGMEM = "TEST FAILED";
	
	bool done_check[5] = { false, false, false, false, false};
	bool result = false;
	unsigned long startzeit = millis();
	unsigned long zeitstempel = startzeit;
	
	logger_g_->LnWriteLog(trennlinie_, debug);
	Zwischenzeit(zeitstempel);
	Zeitstempel(zeitstempel);
	if (Simulate_FirstClick())
	{
		logger_g_->LnWriteLog(buffer_succ, debug);
	}
	else
	{
		logger_g_->LnWriteLog(buffer_fail, debug);
	}
#ifdef MEMINFO_
	free_mem();
#endif
	zeitstempel = millis();
	while (millis() <= testtime_)
	{
		if (millis() - zeitstempel >= new_switch_controller.time_between_trigger && !done_check[0])
		{
			logger_g_->LnWriteLog(trennlinie_, debug);
			Zwischenzeit(zeitstempel);
			Zeitstempel(zeitstempel);
			if (Simulate_SelectedNumberOfClicks(1, 1, startzeit))
			{
				logger_g_->LnWriteLog(buffer_succ, debug);
			}
			else
			{
				logger_g_->LnWriteLog(buffer_fail, debug);
			}
			zeitstempel = millis();
			done_check[0] = true;
#ifdef MEMINFO_
			free_mem();
#endif
		}
		if (millis() - zeitstempel >= new_switch_controller.time_between_trigger && !done_check[1] && done_check[0])
		{
			logger_g_->LnWriteLog(trennlinie_, debug);
			Zwischenzeit(zeitstempel);
			Zeitstempel(zeitstempel);
			if (Simulate_SelectedNumberOfClicks(2, 2,startzeit))
			{
				logger_g_->LnWriteLog(buffer_succ, debug);
			}
			else
			{
				logger_g_->LnWriteLog(buffer_fail, debug);
			}
			zeitstempel = millis();
			done_check[1] = true;
#ifdef MEMINFO_
			free_mem();
#endif
		}
		if (millis() - zeitstempel >= new_switch_controller.time_between_trigger && !done_check[2] && done_check[1])
		{
			logger_g_->LnWriteLog(trennlinie_, debug);
			Zwischenzeit(zeitstempel);
			Zeitstempel(zeitstempel);
			if (Simulate_WrongInput1(startzeit))
			{
				logger_g_->LnWriteLog(buffer_succ, debug);
			}
			else
			{
				logger_g_->LnWriteLog(buffer_fail, debug);
			}
			zeitstempel = millis();
			done_check[2] = true;
#ifdef MEMINFO_
			free_mem();
#endif
		}
		if (millis() - zeitstempel >= new_switch_controller.time_between_trigger && !done_check[3] && done_check[2])
		{
			logger_g_->LnWriteLog(trennlinie_, debug);
			Zwischenzeit(zeitstempel);
			Zeitstempel(zeitstempel);
			if (Simulate_WrongInput2(startzeit))
			{
				logger_g_->LnWriteLog(buffer_succ, debug);
			}
			else
			{
				logger_g_->LnWriteLog(buffer_fail, debug);
			}
			zeitstempel = millis();
			done_check[3] = true;
#ifdef MEMINFO_
			free_mem();
#endif
		}
		if (millis() - zeitstempel >= new_switch_controller.time_between_trigger && !done_check[4] && done_check[3])
		{
			logger_g_->LnWriteLog(trennlinie_, debug);
			Zwischenzeit(zeitstempel);
			Zeitstempel(zeitstempel);
			if (Simulate_LastClick(startzeit))
			{
				logger_g_->LnWriteLog(buffer_succ, debug);
			}
			else
			{
				logger_g_->LnWriteLog(buffer_fail, debug);
			}
			zeitstempel = millis();
			done_check[4] = true;
#ifdef MEMINFO_
			free_mem();
#endif
		}
	}
}

bool Test_SwitchController::Simulate_SelectedNumberOfClicks(unsigned int eventtype, unsigned int clickcount, unsigned long startzeit)
{
#ifdef DEBUG
	static const char* const buffer PROGMEM = "Call - Simulate_SelectedNumberOfClicks";
	logger_g_->LnWriteLog(buffer, extremedebug);
#endif
#ifdef MEMINFO_
	free_mem();
#endif
	bool result = false;
	unsigned long zeitstempel = millis();

	logger_g_->LnWriteLog(trigger_array_[eventtype], debug);
	static const char* const buffer1 PROGMEM = " wird getestet";
	logger_g_->WriteLog(buffer1, debug);

	if (new_switch_controller.is_on)
	{
		static const char* const buffer2 PROGMEM = "Press Button";
		logger_g_->LnWriteLog(buffer2, debug);
		result = new_switch_controller.Event_handler(true, millis());
		zeitstempel = millis();

		bool toggle = false;
		for (int i = 1; i < clickcount * 2; ++i)
		{
			if (toggle == false)
			{
				result = DoIfTimeForButtonRelease(eventtype, 0, millis(), result);
				toggle = true;
			}
			else
			{
				result = DoIfTimeForButtonPress(eventtype, 0, millis(), result);
				toggle = false;
			}
		}
	}
	else
	{
		static const char* const buffer3 PROGMEM = "Testerror, Zustand ist 'Off'";
		logger_g_->WriteLog(buffer3, debug);
	}
	return Validate(result, trigger_array_[eventtype]);
}

bool Test_SwitchController::DoIfTimeForButtonRelease(unsigned short idx, short additive, unsigned long zeitstempel, bool result)
{
#ifdef DEBUG
	static const char* const buffer_fail PROGMEM = "Call - DoIfTimeForButtonRelease";
	logger_g_->LnWriteLog(buffer_fail, extremedebug);
#endif
#ifdef MEMINFO_
	free_mem();
#endif
	do {
		Zwischenzeit(zeitstempel);
		if (millis() - zeitstempel + additive >= new_switch_controller.events[idx].prop_continuous_hold)
		{
			Zwischenzeit(zeitstempel);
			static const char* const buffer PROGMEM = "Release Button";
			logger_g_->LnWriteLog(buffer, debug);
			result = new_switch_controller.Event_handler(false, millis());
			zeitstempel = millis();
			break;
		}
	} while (millis() - zeitstempel < new_switch_controller.events[idx].prop_continuous_hold + 500);
	return result;
}

bool Test_SwitchController::DoIfTimeForButtonPress(unsigned short idx, short additive, unsigned long zeitstempel, bool result)
{
#ifdef DEBUG
	static const char* const buffer PROGMEM = "Call - DoIfTimeForButtonPress";
	logger_g_->LnWriteLog(buffer, extremedebug);
#endif
#ifdef MEMINFO_
	free_mem();
#endif;
	do {
		Zwischenzeit(zeitstempel);
		if (millis() - zeitstempel + additive >= new_switch_controller.events[idx].prop_threshhold_between)
		{
			Zwischenzeit(zeitstempel);
			static const char* const buffer PROGMEM = "Press Button";
			logger_g_->LnWriteLog(buffer, debug);
			result = new_switch_controller.Event_handler(true, millis());
			zeitstempel = millis();
			break;
		}
	} while (millis() - zeitstempel < new_switch_controller.events[idx].prop_threshhold_between + 500);
	return result;
}

bool Test_SwitchController::Simulate_FirstClick()
{
#ifdef DEBUG

	static const char* const buffer PROGMEM = "Call - Simulate_FirstClick";
	logger_g_->LnWriteLog(buffer, extremedebug);
#endif
#ifdef MEMINFO_
	free_mem();
#endif
	bool result = false;
	unsigned long zeitstempel = millis();
	
	logger_g_->LnWriteLog(trigger_array_[0], debug);
	static const char* const buffer1 PROGMEM = " wird getestet";
	logger_g_->WriteLog(buffer1, debug);

	if (!new_switch_controller.is_on)
	{
		static const char* const buffer2 PROGMEM = "Press Button";
		logger_g_->LnWriteLog(buffer2, debug);
		
		result = new_switch_controller.Event_handler(true, millis());
		zeitstempel = millis();
		result = DoIfTimeForButtonRelease(0, 0, millis(), result);
	}
	else
	{
		logger_g_->LnWriteLog(trigger_array_[0], debug);
		static const char* const buffer PROGMEM = "Testerror, Zustand ist schon 'On' ";
		logger_g_->WriteLog(buffer, debug);
	}
	return Validate(result, trigger_array_[0]);
}

bool Test_SwitchController::Simulate_LastClick(unsigned long startzeit)
{
#ifdef DEBUG
	static const char* const buffer PROGMEM = "Call - Simulate_LastClick";
	logger_g_->LnWriteLog(buffer, extremedebug);
#endif
#ifdef MEMINFO_
	free_mem();
#endif
	
	bool result = false;
	unsigned long zeitstempel = millis();

	logger_g_->LnWriteLog(trigger_array_[3], debug);
	static const char* const buffer1 PROGMEM = " wird getestet";
	logger_g_->WriteLog(buffer1, debug);
	
	if (new_switch_controller.is_on)
	{
		static const char* const buffer2 PROGMEM = "Press Button";
		logger_g_->LnWriteLog(buffer2, debug);
		
		result = new_switch_controller.Event_handler(true, millis());
		zeitstempel = millis();

		result = DoIfTimeForButtonRelease(3, 0, millis(), result);
	}
	else
	{
		logger_g_->LnWriteLog(trigger_array_[3], debug);
		static const char* const buffer3 PROGMEM = " Testerror, Zustand ist 'Off' ";
		logger_g_->WriteLog(buffer3, debug);
	}

	return Validate(result, trigger_array_[3]);
}

bool Test_SwitchController::Simulate_WrongInput1(unsigned long startzeit)
{
#ifdef DEBUG
	static const char* const buffer PROGMEM = "Call - Simulate_WrongInput1";
	logger_g_->LnWriteLog(buffer, extremedebug);
#endif
#ifdef MEMINFO_
	free_mem();
#endif
	
	bool result = false;
	unsigned long zeitstempel = millis();

	static const char* const buffer1 PROGMEM = "Fehlerhafte Eingabe Test (zu lange gedrueckt)";
	logger_g_->LnWriteLog(buffer1, debug);
	
	if (new_switch_controller.is_on)
	{
		static const char* const buffer2 PROGMEM = "Press Button";
		logger_g_->LnWriteLog(buffer2, debug);
		
		result = new_switch_controller.Event_handler(true, millis());
		zeitstempel = millis();

		result = DoIfTimeForButtonRelease(1, -500, millis(), result);
	}
	else
	{
		logger_g_->LnWriteLog(trigger_array_[1], debug);
		static const char* const buffer3 PROGMEM = " Testerror, Zustand ist 'Off'";
		logger_g_->WriteLog(buffer3, debug);
		return false;
	}

	return ValidateNegative(result, trigger_array_[1]);
}

bool Test_SwitchController::Simulate_WrongInput2(unsigned long startzeit)
{
#ifdef DEBUG
	static const char* const buffer PROGMEM = "Call - Simulate_WrongInput2";
	logger_g_->LnWriteLog(buffer, extremedebug);
#endif
#ifdef MEMINFO_
	free_mem();
#endif
	
	bool result = false;
	unsigned long zeitstempel = millis();

	static const char* const buffer1 PROGMEM = "Fehlerhafte eingabe Test (zu lange gedrueckt)";
	logger_g_->LnWriteLog(buffer1, debug);

	if (new_switch_controller.is_on)
	{
		static const char* const buffer2 PROGMEM = "Press Button";
		logger_g_->LnWriteLog(buffer2, debug);
		
		result = new_switch_controller.Event_handler(true, millis());
		zeitstempel = millis();

		result = DoIfTimeForButtonRelease(2, -300, millis(), result);
		result = DoIfTimeForButtonPress(2, 400, millis(), result);
		result = DoIfTimeForButtonRelease(2, -500, millis(), result);
	}
	else
	{
		logger_g_->LnWriteLog(trigger_array_[2], debug);
		static const char* const buffer3 PROGMEM = " Testerror, Zustand ist 'Off'";
		logger_g_->WriteLog(buffer3, debug);
		return false;
	}

	return ValidateNegative(result, trigger_array_[2]);
}

bool Test_SwitchController::Validate(bool vali, const char name_of_trigger[])
{
#ifdef DEBUG
	static const char* const buffer PROGMEM = "Call - Validate";
	logger_g_->LnWriteLog(buffer, extremedebug);
#endif
#ifdef MEMINFO_
	free_mem();
#endif
	if (vali)
	{
		for (int idx = 0; idx < new_switch_controller.events.size(); ++idx)
		{
			//if (new_switch_controller.events[idx].prop_event_title == name_of_trigger && new_switch_controller.events[idx].vali_is_triggered_)
			//{
			//	vreturn = true;
			//	logger_g_->LnWriteLog(new_switch_controller.events[idx].prop_event_title, debug);
			//	static const char* const buffer PROGMEM = " wurde ausgeloest";
			//	logger_g_->WriteLog(buffer, debug);
			//}

			if (new_switch_controller.events[idx].prop_event_title != name_of_trigger && new_switch_controller.events[idx].vali_is_triggered_)
			{
				vali = false;
				logger_g_->LnWriteLog(new_switch_controller.events[idx].prop_event_title, debug);
				static const char* const buffer PROGMEM = " wurde ausgeloest";

			}
		}
		if (!vali)
		{
			static const char* const buffer PROGMEM = " Es wurden mehrere Events ausgeloest";
			logger_g_->LnWriteLog(buffer, debug);
		}
	}
	else
	{
		vali = false;
		static const char* const buffer PROGMEM = "Es wurde kein Event ausgeloest";
		logger_g_->LnWriteLog(buffer, debug);
	}

	return vali;
}

bool Test_SwitchController::ValidateNegative(bool vali, const char name_of_trigger[])
{
#ifdef DEBUG
	static const char* const buffer PROGMEM = "Call - ValidateNegative";
	logger_g_->LnWriteLog(buffer, extremedebug);
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
				logger_g_->LnWriteLog(new_switch_controller.events[idx].prop_event_title, debug);
				static const char* const buffer PROGMEM = " wurde faelschlich ausgeloest";
				logger_g_->WriteLog(buffer, debug);
			}
		}
	}
	else
	{
		vali = true;
		static const char* const buffer PROGMEM = "Es wurde kein Event ausgeloest";
		logger_g_->LnWriteLog(buffer, debug);
	}

	return vali;
}

bool Test_SwitchController::ValidateTimeout(bool vali)
{
#ifdef DEBUG
	static const char* const buffer PROGMEM = "Call - ValidateTimeout";
	logger_g_->LnWriteLog(buffer, extremedebug);
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
				static const char* const buffer PROGMEM = "vali_is_triggered_ ist resettiert";
				logger_g_->LnWriteLog(buffer, debug);
			}
			else
			{
				vali = false;
			}

			if (new_switch_controller.events[idx].vali_triggered_time = 0)
			{
				vali = true;
				static const char* const buffer PROGMEM = "vali_triggered_time ist resettiert";
				logger_g_->LnWriteLog(buffer, debug);
			}
			else
			{
				vali = false;
			}

			if (new_switch_controller.events[idx].vali_is_failed_ = false)
			{
				vali = true;
				static const char* const buffer PROGMEM = "vali_is_failed_ ist resettiert";
				logger_g_->LnWriteLog(buffer, debug);
			}
			else
			{
				vali = false;
			}
		}
	}
	return vali;
}

void Test_SwitchController::Zwischenzeit(unsigned long zeitstempel)
{
#ifdef MEMINFO_
	free_mem();
#endif
	static const char* const buffer PROGMEM = "Zwischenzeit: ";
	logger_g_->LnWriteLog(buffer, debug);
	logger_g_->WriteLog(millis() - zeitstempel, debug);
	//delete buffer;
}
;
void Test_SwitchController::Zeitstempel(unsigned long zeitstempel)
{
#ifdef MEMINFO_
	free_mem();
#endif
	static const char* const buffer PROGMEM = "Zeitstempel: ";
	logger_g_->LnWriteLog(buffer, debug);
	logger_g_->WriteLog(zeitstempel, debug);
	//delete buffer;
}