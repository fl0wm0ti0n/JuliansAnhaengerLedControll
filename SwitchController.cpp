// 
// 
// 
#include "SwitchController.h"
#include "MemoryInfo.h"

SwitchController::SwitchController()
{
}

SwitchController::SwitchController(unsigned short timeout, unsigned short time_between_trigger)
{

#ifdef DEBUG
	logger_g_ = logger::GetInstance();
	static const char* const logbuffer PROGMEM = "Call - SwitchController";
	logger_g_->LnWriteLog(logbuffer, extremedebug);
#endif
	start_time_ = millis();
	this->timeout = timeout;
	this->time_between_trigger = time_between_trigger;
}

SwitchController::~SwitchController()
= default;

/**
 * \brief handling a button press
 * \param timestamp exactly timestamp of pressing the button
 * \return bool
 */
bool SwitchController::Event_handler_press(unsigned long timestamp)
{
#ifdef DEBUG
	static const char* const buffer PROGMEM = "Call - E_h_press";
	logger_g_->LnWriteLog(buffer, extremedebug);
#endif
#ifdef MEMINFO_
	free_mem();
#endif
	// set actual button press time
	press_time_ = timestamp;
	// calculate time between last buttone release and actual button press
	auto freetime = (press_time_ - release_time_);
	// reset if input period timed out
	reset_if_no_input_over_long_time(freetime);
	// count button press
	press_count_++;
	// check all events if event trigger got triggered
	for (int idx = 0; idx < events.size(); ++idx)
	{
		//check prop_threshhold_between (wenn 2. click) -> validated_press_++
		if (press_count_ > 1 && !events[idx].prop_is_first_click)
		{
			//static const char* const buffer1 PROGMEM = "<--- HIER 1";
			//logger_g_->LnWriteLog(buffer1, debug);
			if (Check_if_valid_threshold(freetime, idx)) break;
		}	
		if (press_count_ < 2 && !events[idx].prop_is_first_click)
		{
			//static const char* const buffer1 PROGMEM = "<--- HIER 2";
			//logger_g_->LnWriteLog(buffer1, debug);
			events[idx].validated_press_++;
			continue;
		}
		if (press_count_ == 1 && events[idx].prop_is_first_click && !is_on)
		{
			//static const char* const buffer1 PROGMEM = "<--- HIER 3";
			//logger_g_->LnWriteLog(buffer1, debug);
			first_click_ = true;
			break;
		}
	}
	return false;
}

/**
 * @brief handling a button release 
 * @param timestamp exactly timestamp of releasing the button
 * @return bool
 */
bool SwitchController::Event_handler_release(unsigned long timestamp)
{
#ifdef DEBUG
	static const char* const buffer PROGMEM = "Call - E_h_release";
	logger_g_->LnWriteLog(buffer, extremedebug);
#endif
#ifdef MEMINFO_
	free_mem();
#endif
	
	char ul_buffer[sizeof(unsigned long) * 8 + 1];
	
	// set actual button release time
	release_time_ = timestamp;
	// count button release
	release_count_++;
	// calculate time between last buttone press and actual button release
	auto holdtime = (release_time_ - press_time_);	
	// check all events if event trigger got triggered
	for (int idx = 0; idx < events.size(); ++idx)
	{
		Serial.println(holdtime);
		Serial.println(idx);
		if (!Check_if_valid_hold(holdtime, idx)) continue;
#ifdef DEBUG
		static const char* const buffer0 PROGMEM = "Event: ";
		logger_g_->LnWriteLog(buffer0, debug);
		logger_g_->WriteLog(events[idx].prop_event_title, debug);
		static const char* const buffer1 PROGMEM = "thb: ";
		logger_g_->LnWriteLog(buffer1, debug);
		logger_g_->WriteLog(events[idx].prop_threshhold_between, debug);
		static const char* const buffer2 PROGMEM = "vhold: ";
		logger_g_->LnWriteLog(buffer2, debug);
		logger_g_->WriteLog(events[idx].validated_hold_, debug);
		static const char* const buffer3 PROGMEM = "vpress: ";
		logger_g_->LnWriteLog(buffer3, debug);
		logger_g_->WriteLog(events[idx].validated_press_, debug);
#endif

		if (Check_if_firstclick(idx)) { set_on(); return true;}
		Add_click_if_hold_and_press_is_even(idx);

		if (Check_if_event_got_triggered(idx))
		{
			if (set_off() || run_event_and_reset()) return true;
			break;
		}
	}
	return false;
}

// raw handling of press or release
bool SwitchController::Event_handler(bool up_down, unsigned long timestamp)
{
#ifdef DEBUG
	static const char* const buffer PROGMEM = "Call - Event_handler";
	logger_g_->LnWriteLog(buffer, extremedebug);
#endif
#ifdef MEMINFO_
	free_mem();
#endif

	if (!flipflop_)
	{
		if (up_down) flipflop_ = true; return Event_handler_press(timestamp);
		return false;
	}
	if (flipflop_)
	{
		if (!up_down) flipflop_ = false; return Event_handler_release(timestamp);
		return false;
	}
}

// check timeout, if out of range -> reset vali & events
void SwitchController::reset_if_no_input_over_long_time(unsigned long freetime)
{
	if (freetime >= time_between_trigger && is_on)
	{
#ifdef DEBUG
		static const char* const buffer PROGMEM = "Call - Event_handler";
		logger_g_->LnWriteLog(buffer, extremedebug);
#endif
#ifdef MEMINFO_
		free_mem();
#endif
		
		reset_events();
		reset_common_validating();
	}
}

// check if the press was valid
bool SwitchController::Check_if_valid_threshold(unsigned long freetime, int idx)
{
#ifdef DEBUG
	static const char* const buffer PROGMEM = "Call - threshold?";
	logger_g_->LnWriteLog(buffer, extremedebug);
	//const char buffer1[] PROGMEM = "freetime: ";
	//logger_g_->LnWriteLog(buffer1, debug);
	//logger_g_->WriteLog(freetime, debug);
#endif
#ifdef MEMINFO_
	free_mem();
#endif
	if (freetime >= events[idx].prop_threshhold_between - events[idx].prop_tolerance && freetime <= events[idx].prop_threshhold_between + events[idx].prop_tolerance)
	{
#ifdef DEBUG
		static const char* const buffer2 PROGMEM = "[is valid threshold]";
		logger_g_->LnWriteLog(buffer2, debug);
#endif
		events[idx].validated_press_++;
		return true;
	}
	events[idx].vali_is_failed_ = true;
	return false;
}

// check if the hold was valid
bool SwitchController::Check_if_valid_hold(unsigned long holdtime, int idx)
{
#ifdef DEBUG
	static const char* const buffer PROGMEM = "Call - hold?";
	logger_g_->LnWriteLog(buffer, extremedebug);
	static const char* const buffer1 PROGMEM = "holdtime: ";
	logger_g_->LnWriteLog(buffer1, debug);
	logger_g_->WriteLog(holdtime, debug);
#endif
#ifdef MEMINFO_
	free_mem();
#endif
	if (holdtime >= events[idx].prop_continuous_hold - events[idx].prop_tolerance && holdtime <= events[idx].prop_continuous_hold + events[idx].prop_tolerance)
	{
		Serial.println(F("valid hold"));
#ifdef DEBUG
		static const char* const buffer2 PROGMEM = "[is valid hold]";

		logger_g_->LnWriteLog(buffer2, debug);
#endif
		events[idx].validated_hold_++;
		return true;
	}
	else
	{
		events[idx].vali_is_failed_ = true;
		return false;
	}
}

// check if firstclick is triggered
bool SwitchController::Check_if_firstclick(int idx)
{
#ifdef DEBUG
	static const char* const buffer PROGMEM = "Call - firstclick?";
	logger_g_->LnWriteLog(buffer, extremedebug);
#endif
#ifdef MEMINFO_
	free_mem();
#endif
	if (first_click_ && events[idx].validated_hold_ > 0 && events[idx].prop_is_first_click && !is_on)
	{
		Serial.println(F("first"));
#ifdef DEBUG
		static const char* const buffer1 PROGMEM = "[is first click]";
		logger_g_->LnWriteLog(buffer1, debug);
#endif
		events[idx].vali_is_triggered_ = true;
		events[idx].vali_triggered_time = millis();
		//set_all_events_failed_except_one(events[idx].prop_event_title);
		return true;
	}
	return false;
}

// check validated_press eq validated_hold -> validated_clicks
void SwitchController::Add_click_if_hold_and_press_is_even(int idx)
{
#ifdef DEBUG
	static const char* const buffer PROGMEM = "Call - AddClick";
	logger_g_->LnWriteLog(buffer, extremedebug);
#endif
#ifdef MEMINFO_
	free_mem();
#endif
	if (events[idx].validated_hold_ > 0 && events[idx].validated_press_ > 0)
	{
#ifdef DEBUG
		static const char* const buffer1 PROGMEM = "hold count: ";
		logger_g_->LnWriteLog(buffer1, debug);
		logger_g_->WriteLog(events[idx].validated_hold_, debug);
		static const char* const buffer2 PROGMEM = "press count: ";
		logger_g_->LnWriteLog(buffer2, debug);
		logger_g_->WriteLog(events[idx].validated_press_, debug);
#endif
		if (events[idx].validated_hold_ == events[idx].validated_press_)
		{
			events[idx].validated_clicks_++;
#ifdef DEBUG
			static const char* const buffer3 PROGMEM = "[added click]";
			logger_g_->LnWriteLog(buffer3, debug);
			static const char* const buffer4 PROGMEM = "count: ";
			logger_g_->LnWriteLog(buffer4, debug);
			logger_g_->WriteLog(events[idx].validated_clicks_, debug);
#endif
		}
	}
}

// check if expected clicks are reached, if yes set event triggered and all others non triggered
bool SwitchController::Check_if_event_got_triggered(int idx)
{
#ifdef DEBUG
	static const char* const buffer PROGMEM = "Call - triggered?";
	logger_g_->LnWriteLog(buffer, extremedebug);
#endif
#ifdef MEMINFO_
	free_mem();
#endif
	if (events[idx].validated_clicks_ == events[idx].prop_clicks && events[idx].validated_clicks_ > 0)
	{
#ifdef DEBUG
		static const char* const buffer1 PROGMEM = "[click number reached]";
		logger_g_->LnWriteLog(buffer1, debug);
#endif
		events[idx].vali_is_triggered_ = true;
		events[idx].vali_triggered_time = millis();
		//set_all_events_failed_except_one(events[idx].prop_event_title);
		return true;
	}
	return false;
}

// reset all events to default state
void SwitchController::reset_events()
{
#ifdef DEBUG
	static const char* const buffer PROGMEM = "Call - rst_events";
	logger_g_->LnWriteLog(buffer, extremedebug);
#endif
#ifdef MEMINFO_
	free_mem();
#endif
	for (int idx = 0; idx < events.size(); ++idx)
	{
		events[idx].vali_is_failed_ = false;
		events[idx].vali_is_triggered_ = false;
		events[idx].vali_triggered_time = 0;
		events[idx].validated_clicks_ = 0;
		events[idx].validated_hold_ = 0;
		events[idx].validated_press_ = 0;
	}
}

// reset all validate variables of class to default
void SwitchController::reset_common_validating()
{
#ifdef DEBUG
	static const char* const buffer PROGMEM = "Call - rst_c_vali";
	logger_g_->LnWriteLog(buffer, extremedebug);
#endif
#ifdef MEMINFO_
	free_mem();
#endif
	press_count_ = 0;
	release_count_ = 0;
	first_click_ = false;
}

// set all events to failed exept the passed one
void SwitchController::set_all_events_failed_except_one(char except[])
{
#ifdef DEBUG
	static const char* const buffer PROGMEM = "Call - set events failed";
	logger_g_->LnWriteLog(buffer, extremedebug);
#endif
#ifdef MEMINFO_
	free_mem();
#endif
	for (int idx = 0; idx < events.size(); ++idx)
	{
		if (events[idx].prop_event_title == except)
		{
#ifdef DEBUG
			static const char* const buffer1 PROGMEM = "excepted event: ";
			logger_g_->LnWriteLog(buffer1, debug);
			logger_g_->WriteLog(except, debug);
#endif
			continue;
		}
		events[idx].vali_is_failed_ = true;
		events[idx].vali_is_triggered_ = false;
		events[idx].vali_triggered_time = 0;
	}
}

// set the eventclass on for other events than only firstclick
bool SwitchController::set_on()
{
#ifdef DEBUG
	static const char* const buffer PROGMEM = "Call - set_on";
	logger_g_->LnWriteLog(buffer, extremedebug);
#endif
#ifdef MEMINFO_
	free_mem();
#endif
	// suche Einschalt event und ob getriggert wurde 
	for (int idx = 0; idx < events.size(); ++idx)
	{
		if (events[idx].prop_is_first_click)
		{
			if (events[idx].vali_is_triggered_)
			{
#ifdef DEBUG
				static const char* const buffer2 PROGMEM = "[EventWatcher On]";
				logger_g_->LnWriteLog(buffer2, debug);
#endif
				first_time_ = millis();
				is_on = true;

				strcpy(last_triggerede_event, events[idx].prop_event_title);
				Serial.println(F("set_on"));
				
				reset_events();
				reset_common_validating();
				return true;
			}
		}
	}
	return false;
}

// set the eventclass to state off that only firstclick will be noticed
bool SwitchController::set_off()
{
#ifdef DEBUG
	static const char* const buffer PROGMEM = "Call - set_off";
	logger_g_->LnWriteLog(buffer, extremedebug);
#endif
#ifdef MEMINFO_
	free_mem();
#endif
	// suche Einschalt event und ob getriggert wurde 
	for (int idx = 0; idx < events.size(); ++idx)
	{
		if (events[idx].prop_is_shutoff_click)
		{
			if (events[idx].vali_is_triggered_)
			{
#ifdef DEBUG
				static const char* const buffer1 PROGMEM = "[EventWatcher Off]";
				logger_g_->LnWriteLog(buffer1, debug);
#endif
				last_time_ = millis();
				is_on = false;

				strcpy(last_triggerede_event, events[idx].prop_event_title);
				Serial.println(F("set_off"));
				
				reset_events();
				reset_common_validating();
				
				return true;
			}
		}
	}
	return false;
}

// run triggered event and reset the whole eventclass
bool SwitchController::run_event_and_reset()
{
#ifdef DEBUG
	static const char* const buffer PROGMEM = "Call - run_event_reset";
	logger_g_->LnWriteLog(buffer, extremedebug);
#endif
#ifdef MEMINFO_
	free_mem();
#endif
	// suche Einschalt event und ob getriggert wurde 
	for (int idx = 0; idx < events.size(); ++idx)
	{
		//logger_g_->LnWriteLog(events[idx].vali_is_triggered_, debug);
		if (events[idx].vali_is_triggered_ && is_on)
		{
#ifdef DEBUG
			static const char* const buffer1 PROGMEM = "[run event and reset]";
			logger_g_->LnWriteLog(buffer1, debug);
#endif

			strcpy(last_triggerede_event, events[idx].prop_event_title);
			Serial.println(F("run_event"));
			
			reset_events();
			reset_common_validating();
			return true;
		}
	}
	return false;
}

// add new event to eventclass, the new event will be watched if it gets triggered
void SwitchController::New_event(const char title[], unsigned long threshhold_between, unsigned short clicks, unsigned long continuous_hold, short tolerance, bool first_click, bool shutoff_click)
{
#ifdef DEBUG
	static const char* const buffer PROGMEM = "Call - New_event";
	logger_g_->LnWriteLog(buffer, extremedebug);
#endif
#ifdef MEMINFO_
	free_mem();
#endif
	bool check = false;
	
	if (events.size() > 0)
	{		
		for (int idx = 0; idx < events.size(); ++idx)
		{
			if (events[idx].prop_event_title != title || events[idx].prop_is_first_click != true || events[idx].prop_is_shutoff_click != true)
			{
				check = true;
			}
			else if (events[idx].prop_is_first_click == true)
			{
				check = false;
#ifdef DEBUG
				static const char* const buffer1 PROGMEM = "Event '";
				logger_g_->LnWriteLog(buffer1, debug);
				logger_g_->WriteLog(events[idx].prop_event_title, debug);
				static const char* const buffer2 PROGMEM = "' fuer ShutOn, ist schon vorhanden";
				logger_g_->WriteLog(buffer2, debug);
#endif
			}
			else if (events[idx].prop_is_shutoff_click == true)
			{
				check = false;
#ifdef DEBUG
				static const char* const buffer3 PROGMEM = "Event '";
				logger_g_->LnWriteLog(buffer3, debug);
				logger_g_->WriteLog(events[idx].prop_event_title, debug);
				static const char* const buffer4 PROGMEM = "' fue ShutOff, ist schon vorhanden";
				logger_g_->WriteLog(buffer4, debug);
#endif
			}
			else if (events[idx].prop_event_title == title)
			{
				check = false;
#ifdef DEBUG
				static const char* const buffer5 PROGMEM = "Event '";
				logger_g_->LnWriteLog(buffer5, debug);
				logger_g_->WriteLog(events[idx].prop_event_title, debug);
				static const char* const buffer6 PROGMEM = "' ist schon vorhanden";
				logger_g_->WriteLog(buffer6, debug);
#endif
			}
		}
	}

	if (events.size() == 0 || check)
	{
		check = false;
#ifdef DEBUG
		static const char* const buffer7 PROGMEM = "Neues Event: ";
		logger_g_->LnWriteLog(buffer7, debug);
		logger_g_->WriteLog(title, debug);
#endif
		EventTrigger new_trigger;
		strcpy(new_trigger.prop_event_title, title);
		new_trigger.prop_clicks = clicks;
		new_trigger.prop_continuous_hold = continuous_hold;
		new_trigger.prop_threshhold_between = threshhold_between;
		new_trigger.prop_is_first_click = first_click;
		new_trigger.prop_is_shutoff_click = shutoff_click;
		new_trigger.prop_tolerance = tolerance;
		events.push_back(new_trigger);
	}
}

void SwitchController::log_detail_valis(unsigned short idx)
{
	static const char* const buffer PROGMEM = "Call - log_detail_valis";
	logger_g_->LnWriteLog(buffer, extremedebug);
#ifdef MEMINFO_
	free_mem();
#endif
	static const char* const buffer1  = "Eventtitle = ";;
	logger_g_->LnWriteLog(buffer1, debug);
	logger_g_->WriteLog(events[idx].prop_event_title, debug);
	static const char* const buffer2 PROGMEM = "validated_clicks_ = ";
	logger_g_->LnWriteLog(buffer2, debug);
	logger_g_->WriteLog(events[idx].validated_clicks_, debug);
	static const char* const buffer3 PROGMEM = "vali_triggered_time = ";
	logger_g_->LnWriteLog(buffer3, debug);
	logger_g_->WriteLog(events[idx].vali_triggered_time, debug);
	static const char* const buffer4 PROGMEM = "validated_press_ = ";
	logger_g_->LnWriteLog(buffer4, debug);
	logger_g_->WriteLog(events[idx].validated_press_, debug);
	static const char* const buffer5 PROGMEM = "validated_hold_ = ";
	logger_g_->LnWriteLog(buffer5, debug);
	logger_g_->WriteLog(events[idx].validated_hold_, debug);
	static const char* const buffer6 PROGMEM = "vali_is_failed_ = ";
	logger_g_->LnWriteLog(buffer6, debug);
	logger_g_->WriteLog(events[idx].vali_is_failed_, debug);
	static const char* const buffer7 PROGMEM = "vali_is_triggered_ = ";
	logger_g_->LnWriteLog(buffer7, debug);
	logger_g_->WriteLog(events[idx].vali_is_triggered_, debug);
	static const char* const buffer8 PROGMEM = "vali_triggered_time = ";
	logger_g_->LnWriteLog(buffer8, debug);
	logger_g_->WriteLog(events[idx].vali_triggered_time, debug);
}

void SwitchController::log_detail_props(unsigned short idx)
{
	static const char* const buffer PROGMEM = "Call - log_detail_props";
	logger_g_->LnWriteLog(buffer, extremedebug);
#ifdef MEMINFO_
	free_mem();
#endif
	
	static const char* const buffer1 PROGMEM = "Eventtitle = ";
	logger_g_->LnWriteLog(buffer1, debug);
	logger_g_->WriteLog(events[idx].prop_event_title, debug);
	static const char* const buffer2 PROGMEM = "prop_clicks = ";
	logger_g_->LnWriteLog(buffer2, debug);
	logger_g_->WriteLog(events[idx].prop_clicks, debug);
	static const char* const buffer3 PROGMEM = "prop_continuous_hold = ";
	logger_g_->LnWriteLog(buffer3, debug);
	logger_g_->WriteLog(events[idx].prop_continuous_hold, debug);
	static const char* const buffer4 PROGMEM = "prop_threshhold_between = ";
	logger_g_->LnWriteLog(buffer4, debug);
	logger_g_->WriteLog(events[idx].prop_threshhold_between, debug);
	static const char* const buffer5 PROGMEM = "prop_tolerance = ";
	logger_g_->LnWriteLog(buffer5, debug);
	logger_g_->WriteLog(events[idx].prop_tolerance, debug);
	static const char* const buffer6 PROGMEM = "prop_is_first_click = ";
	logger_g_->LnWriteLog(buffer6, debug);
	logger_g_->WriteLog(events[idx].prop_is_first_click, debug);
	static const char* const buffer7 PROGMEM = "prop_is_last_click =";
	logger_g_->LnWriteLog(buffer7, debug);
	logger_g_->WriteLog(events[idx].prop_is_shutoff_click, debug);
}

void SwitchController::Zwischenzeit(unsigned long zeitstempel)
{
#ifdef MEMINFO_
	free_mem();
#endif
	static const char* const buffer PROGMEM = "Zwischenzeit: ";
	logger_g_->LnWriteLog(buffer, debug);
	logger_g_->WriteLog(millis() - zeitstempel, debug);
}