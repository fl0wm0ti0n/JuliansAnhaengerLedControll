// 
// 
// 

#include "SwitchController.h"

SwitchController::SwitchController()
{
}

SwitchController::SwitchController(unsigned short timeout, unsigned short time_between_trigger)
{
	logger_g_ = logger::GetInstance();
#ifdef _DEBUG
	logger_g_->LnWriteLog(F("Call - SwitchController"), extremedebug);
#endif
	start_time_ = millis();
	this->timeout = timeout;
	this->time_between_trigger = time_between_trigger;
}


SwitchController::~SwitchController()
= default;


bool SwitchController::Event_handler_press(unsigned long timestamp)
{
#ifdef _DEBUG
	logger_g_->LnWriteLog(F("Call - E_h_press"), extremedebug);
#endif

	// Pruefe ob erster click
	//is_first_click();

	press_time_ = millis();
	press_count_++;
	
	// berechne Zeit zwischen letztem release und aktuellem press
	auto freetime = (press_time_ - release_time_);

	//check timeout (wenn >) -> reset

#ifdef _DEBUG
	logger_g_->LnWriteLog(F("freetime = "), extremedebug);
	logger_g_->WriteLog(String(freetime), extremedebug);
#endif
	if (freetime >= time_between_trigger && is_on)
	{
#ifdef _DEBUG
		logger_g_->LnWriteLog(F("time_between_trigger exited"), extremedebug);
#endif
		reset_events();
		reset_common_validating();
	}
	
	// für alle Events
	for (int idx = 0; idx < events.size(); ++idx)
	{
//#ifdef _DEBUG
//		logger_g_->LnWriteLog(F("Eventtitle = "), debug);
//		logger_g_->WriteLog(String(events[idx].prop_event_title), debug);
//#endif
		// Wenn eines davon entgültig zutrifft -> break & vali_is_triggered_ = true & alle anderen vali_is_failed_ = true
		//

		//check prop_threshhold_between (wenn 2. click) -> validated_press_++
		if (press_count_ > 1 && !events[idx].prop_is_first_click)
		{
#ifdef _DEBUG
			logger_g_->LnWriteLog(F("Entered - press_count_ > 1 && !is_first_click"), debug);
			log_detail_valis(idx);
#endif
			if (freetime >= events[idx].prop_threshhold_between - events[idx].prop_tolerance && freetime <= events[idx].prop_threshhold_between + events[idx].prop_tolerance)
			{
#ifdef _DEBUG
				logger_g_->LnWriteLog(F("Entered - freetime = threshhold_between"), debug);
				log_detail_valis(idx);
#endif
				events[idx].validated_press_++;
			}
			else
			{
				events[idx].vali_is_failed_ = true;
			}
		}
		else if (press_count_ < 2 && !events[idx].prop_is_first_click)
		{
			events[idx].validated_press_++;
		}
		else if (press_count_ == 1 && events[idx].prop_is_first_click && !is_on)
		{
#ifdef _DEBUG
			logger_g_->LnWriteLog(F("Entered - press_count_ == 1 && is_first_click && !is_on"), debug);
			logger_g_->LnWriteLog(F("first_click_ = "), debug);
			logger_g_->WriteLog(String(first_click_), debug);
			logger_g_->LnWriteLog(F("is_on = "), debug);
			logger_g_->WriteLog(String(is_on), debug);
			log_detail_valis(idx);
#endif
			first_click_ = true;
			break;
		}
		
		//TODO //check !time_between_trigger_ (wenn < ) -> NIX
	}

	return false;
}


bool SwitchController::Event_handler_release(unsigned long timestamp)
{
#ifdef _DEBUG
	logger_g_->LnWriteLog(F("Call - E_h_release"), extremedebug);
#endif
	
	release_time_ = millis();
	release_count_++;

	// berechne Zeit zwischen letztem press und aktuellem release
	auto holdtime = (release_time_ - press_time_);
#ifdef _DEBUG
	logger_g_->LnWriteLog(F("holdtime = "), extremedebug);
	logger_g_->WriteLog(String(holdtime), extremedebug);
#endif
	// für alle Events
	for (int idx = 0; idx < events.size(); ++idx)
	{

//#ifdef _DEBUG
//		logger_g_->LnWriteLog(F("Eventtitle = "), debug);
//		logger_g_->WriteLog(String(events[idx].prop_event_title), debug);
//#endif

		// Wenn eines davon entgültig zutrifft -> break & vali_is_triggered_ = true & alle anderen vali_is_failed_ = true

		//check prop_continuous_hold (immer) -> validated_hold++
		if (holdtime >= events[idx].prop_continuous_hold - events[idx].prop_tolerance && holdtime <= events[idx].prop_continuous_hold + events[idx].prop_tolerance)
		{
#ifdef _DEBUG
			logger_g_->LnWriteLog(F("Entered - holdtime = continuous_hold"), debug);
			log_detail_valis(idx);
#endif
			events[idx].validated_hold_++;
		}
		else
		{
			events[idx].vali_is_failed_ = true;
		}

		//TODO //check prop_is_first_click (wenn true) -> firstclick_event true -> ON
		if (first_click_ && events[idx].validated_hold_ > 0 && events[idx].prop_is_first_click && !is_on)
		{
#ifdef _DEBUG
			logger_g_->LnWriteLog(F("Entered - first_click_ && validated_hold_ > 0 && is_first_click && !is_on"), debug);
			logger_g_->LnWriteLog(F("first_click_ = "), debug);
			logger_g_->WriteLog(String(first_click_), debug);

			log_detail_valis(idx);
#endif
			events[idx].vali_is_triggered_ = true;
			events[idx].vali_triggered_time = millis();
			set_all_events_failed_except_one(events[idx].prop_event_title);

#ifdef _DEBUG
			log_detail_valis(idx);
			logger_g_->LnWriteLog(F("is_on = "), debug);
			logger_g_->WriteLog(String(is_on), debug);
#endif
			break;
		}
		
		//check validated_press eq validated_hold -> validated_clicks
		if (events[idx].validated_hold_ > 0 && events[idx].validated_press_ > 0)
		{
			if (events[idx].validated_hold_ == events[idx].validated_press_)
			{
#ifdef _DEBUG
				logger_g_->LnWriteLog(F("Entered - validated_hold_ == validated_press_"), debug);
				log_detail_valis(idx);
#endif
				events[idx].validated_clicks_++;
			}
		}

		//TODO // Prüfe ob clicks erreicht?
		if (events[idx].validated_clicks_ == events[idx].prop_clicks)
		{
#ifdef _DEBUG
			logger_g_->LnWriteLog(F("Entered - validated_clicks_ == clicks"), debug);
			logger_g_->LnWriteLog(F("is_on = "), debug);
			logger_g_->WriteLog(String(is_on), debug);
			log_detail_valis(idx);
#endif
			events[idx].vali_is_triggered_ = true;
			events[idx].vali_triggered_time = millis();
			set_all_events_failed_except_one(events[idx].prop_event_title);
			break;
		}
	
		//TODO //check prop_is_row_trigger, wenn event mit diesem Flag auslöst wird jeder weitere click bei dem die Thresholds passen auslösen, bis threshold nicht passt. Alle anderen events werden solange ruhiggestellt.

	}

	if (set_on() || set_off() || run_event_and_reset())
	{
		logger_g_->LnWriteLog(F("is_on = "), debug);
		logger_g_->WriteLog(String(is_on), debug);
		return true;
	}
	return false;
}


bool SwitchController::Event_handler(bool up_down, unsigned long timestamp)
{
#ifdef _DEBUG
	logger_g_->LnWriteLog(F("Call - Event_handler"), extremedebug);
#endif

	if (up_down)
	{
		return Event_handler_press(timestamp);
	}
	return Event_handler_release(timestamp);
}

void SwitchController::reset_events()
{
#ifdef _DEBUG
	logger_g_->LnWriteLog(F("Call - reset_events"), extremedebug);
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


void SwitchController::reset_common_validating()
{
#ifdef _DEBUG
	logger_g_->LnWriteLog(F("Call - reset_common_validating"), extremedebug);
#endif

	press_count_ = 0;
	release_count_ = 0;
	first_click_ = false;
}


void SwitchController::set_all_events_failed_except_one(String except)
{
#ifdef _DEBUG
	logger_g_->LnWriteLog(F("Call - set_all_events_failed_except_one"), extremedebug);
#endif

	for (int idx = 0; idx < events.size(); ++idx)
	{
		if (events[idx].prop_event_title == except)
		{
			continue;
		}
		events[idx].vali_is_failed_ = false;
		events[idx].vali_is_triggered_ = false;
		events[idx].vali_triggered_time = 0;
	}
}


bool SwitchController::set_on()
{
#ifdef _DEBUG
	logger_g_->LnWriteLog(F("Call - set_on"), extremedebug);
#endif

	// suche Einschalt event und ob getriggert wurde 
	for (int idx = 0; idx < events.size(); ++idx)
	{
		if (events[idx].prop_is_first_click)
		{
			if (events[idx].vali_is_triggered_)
			{
				first_time_ = millis();
				is_on = true;
				
				reset_events();
				reset_common_validating();
				
				return true;
			}
			return false;
		}
	}
}

bool SwitchController::set_off()
{
#ifdef _DEBUG
	logger_g_->LnWriteLog(F("Call - set_off"), extremedebug);
#endif

	// suche Einschalt event und ob getriggert wurde 
	for (int idx = 0; idx < events.size(); ++idx)
	{
		if (events[idx].prop_is_shutoff_click)
		{
			if (events[idx].vali_is_triggered_)
			{
				last_time_ = millis();
				is_on = false;
				
				reset_events();
				reset_common_validating();
				
				return true;
			}
			return false;
		}
	}
}

bool SwitchController::run_event_and_reset()
{
#ifdef _DEBUG
	logger_g_->LnWriteLog(F("Call - run_event_and_reset"), extremedebug);
#endif
	
	// suche Einschalt event und ob getriggert wurde 
	for (int idx = 0; idx < events.size(); ++idx)
	{
		if (events[idx].vali_is_triggered_ && is_on)
		{
			reset_events();
			reset_common_validating();
			return true;
		}
		return false;
	}
}


void SwitchController::is_last_click()
{
#ifdef _DEBUG
	logger_g_->LnWriteLog(F("Call - is_last_click"), extremedebug);
#endif
	
	last_time_ = millis();
	is_on = false;
}


void SwitchController::New_event(String title, unsigned long threshhold_between, byte clicks, unsigned long continuous_hold, short tolerance, bool first_click, bool shutoff_click)
{
#ifdef _DEBUG
	logger_g_->LnWriteLog(F("Call - New_event"), extremedebug);
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
				logger_g_->LnWriteLog(F("Event '"), debug);
				logger_g_->WriteLog(events[idx].prop_event_title, debug);
				logger_g_->WriteLog(F("' fuer ShutOn, ist schon vorhanden"), debug);
			}
			else if (events[idx].prop_is_shutoff_click == true)
			{
				check = false;
				logger_g_->LnWriteLog(F("Event '"), debug);
				logger_g_->WriteLog(events[idx].prop_event_title, debug);
				logger_g_->WriteLog(F("' fue ShutOff, ist schon vorhanden"), debug);
			}
			else if (events[idx].prop_event_title == title)
			{
				check = false;
				logger_g_->LnWriteLog(F("Event '"), debug);
				logger_g_->WriteLog(events[idx].prop_event_title, debug);
				logger_g_->WriteLog(F("' ist schon vorhanden"), debug);
			}
		}
	}

	if (events.size() == 0 || check)
	{
		logger_g_->LnWriteLog(F("Neues Event: "), debug);
		logger_g_->WriteLog(String(title), debug);
		
		EventTrigger new_trigger;
		new_trigger.prop_event_title = title;
		new_trigger.prop_clicks = clicks;
		new_trigger.prop_continuous_hold = continuous_hold;
		new_trigger.prop_threshhold_between = threshhold_between;
		new_trigger.prop_is_first_click = first_click;
		new_trigger.prop_is_shutoff_click = shutoff_click;
		new_trigger.prop_tolerance = tolerance;
		events.push_back(new_trigger);
	}
}

void SwitchController::log_detail_valis(byte idx)
{
	logger_g_->LnWriteLog(F("Call - log_detail_valis"), extremedebug);

	logger_g_->LnWriteLog(F("Eventtitle = "), debug);
	logger_g_->WriteLog(String(events[idx].prop_event_title), debug);
	logger_g_->LnWriteLog(F("validated_clicks_ = "), debug);
	logger_g_->WriteLog(String(events[idx].validated_clicks_), debug);
	logger_g_->LnWriteLog(F("vali_triggered_time = "), debug);
	logger_g_->WriteLog(String(events[idx].vali_triggered_time), debug);
	logger_g_->LnWriteLog(F("validated_press_ = "), debug);
	logger_g_->WriteLog(String(events[idx].validated_press_), debug);
	logger_g_->LnWriteLog(F("validated_hold_ = "), debug);
	logger_g_->WriteLog(String(events[idx].validated_hold_), debug);
	logger_g_->LnWriteLog(F("vali_is_failed_ = "), debug);
	logger_g_->WriteLog(String(events[idx].vali_is_failed_), debug);
	logger_g_->LnWriteLog(F("vali_is_triggered_ = "), debug);
	logger_g_->WriteLog(String(events[idx].vali_is_triggered_), debug);
	logger_g_->LnWriteLog(F("vali_triggered_time = "), debug);
	logger_g_->WriteLog(String(events[idx].vali_triggered_time), debug);
}

void SwitchController::log_detail_props(byte idx)
{
	logger_g_->LnWriteLog(F("Call - log_detail_props"), extremedebug);

	logger_g_->LnWriteLog(F("Eventtitle = "), debug);
	logger_g_->WriteLog(String(events[idx].prop_event_title), debug);
	logger_g_->LnWriteLog(F("prop_clicks = "), debug);
	logger_g_->WriteLog(String(events[idx].prop_clicks), debug);
	logger_g_->LnWriteLog(F("prop_continuous_hold = "), debug);
	logger_g_->WriteLog(String(events[idx].prop_continuous_hold), debug);
	logger_g_->LnWriteLog(F("prop_threshhold_between = "), debug);
	logger_g_->WriteLog(String(events[idx].prop_threshhold_between), debug);
	logger_g_->LnWriteLog(F("prop_tolerance = "), debug);
	logger_g_->WriteLog(String(events[idx].prop_tolerance), debug);
	logger_g_->LnWriteLog(F("prop_is_first_click = "), debug);
	logger_g_->WriteLog(String(events[idx].prop_is_first_click), debug);
	logger_g_->LnWriteLog(F("prop_is_last_click = "), debug);
	logger_g_->WriteLog(String(events[idx].prop_is_shutoff_click), debug);
}