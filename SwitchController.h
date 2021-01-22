// SwitchController.h

#ifndef _SWITCHCONTROLLER_h
#define _SWITCHCONTROLLER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "logger.h"
#include "vector.h"


//using std::vector;


struct EventTrigger;

class SwitchController
{

 public:
	SwitchController();
	SwitchController(unsigned short timeout, unsigned short time_between_trigger);
	~SwitchController();
	bool Event_handler_press(unsigned long timestamp);
	bool Event_handler_release(unsigned long timestamp);
	bool Event_handler(bool up_down, unsigned long timestamp);
	void New_event(String title, unsigned long threshhold_between, byte clicks, unsigned long continuous_hold, short tolerance, bool first_click, bool shutoff_click);
	void log_detail_valis(byte idx);
	void log_detail_props(byte idx);
	
	Vector<EventTrigger>	events;
	unsigned short			time_between_trigger	= 0;
	unsigned short			timeout					= 0;
	bool					is_on					= false;

private:
	logger*					logger_g_;
	unsigned long			start_time_				= 0;
	unsigned long			first_time_				= 0;
	unsigned long			last_time_				= 0;
	byte					press_count_			= 0;
	byte					release_count_			= 0;
	unsigned long			release_time_			= 0;
	bool					first_click_			= false;
	unsigned long			press_time_				= 0;


	bool run_event_and_reset();
	bool set_on();
	bool set_off();
	void is_last_click();
	void reset_events();
	void reset_common_validating();
	void set_all_events_failed_except_one(String except);

};


struct EventTrigger
{
	String			prop_event_title;
	unsigned long	prop_threshhold_between;					// millisecond
	byte			prop_clicks;								// Times
	unsigned long	prop_continuous_hold;						// millisecond
	short			prop_tolerance;								// millisecond
			 bool	prop_is_first_click;						// event if false or true
			 bool	prop_is_shutoff_click;						// event if false or true

	// Validate
	bool			vali_is_triggered_				= false;	// result = true if triggered
	bool			vali_is_failed_					= false;	// result = true if triggered
	unsigned long	vali_triggered_time				= 0;
	byte			validated_clicks_				= 0;		// Times
	byte			validated_hold_					= 0;		// Times
	byte			validated_press_				= 0;		// Times
	
};

#endif