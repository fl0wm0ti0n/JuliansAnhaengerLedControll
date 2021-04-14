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
	bool Event_handler(bool up_down, unsigned long timestamp);
	void New_event(const char title[], unsigned long threshhold_between, unsigned short clicks, unsigned long continuous_hold, short tolerance, bool first_click, bool shutoff_click);
	void log_detail_valis(unsigned short idx);
	void log_detail_props(unsigned short idx);
	
	Vector<EventTrigger>	events;
	unsigned short			time_between_trigger	= 0;
	unsigned short			timeout					= 0;
	bool					is_on					= false;
	char					last_triggerede_event[16];
	
private:
	logger*					logger_g_;
	unsigned long			start_time_				= 0;
	unsigned long			first_time_				= 0;
	unsigned long			last_time_				= 0;
	unsigned short			press_count_			= 0;
	unsigned short			release_count_			= 0;
	unsigned long			release_time_			= 0;
	bool					first_click_			= false;
	unsigned long			press_time_				= 0;
	bool					flipflop_				= false;

	void reset_if_no_input_over_long_time(unsigned long freetime);
	bool Event_handler_press(unsigned long timestamp);
	bool Check_if_valid_threshold(unsigned long freetime, int idx);
	bool Check_if_valid_hold(unsigned long holdtime, int idx);
	bool Check_if_firstclick(int idx);
	void Add_click_if_hold_and_press_is_even(int idx);
	bool Check_if_event_got_triggered(int idx);
	bool Event_handler_release(unsigned long timestamp);
	bool run_event_and_reset();
	bool set_on();
	bool set_off();
	void reset_events();
	void reset_common_validating();
	void set_all_events_failed_except_one(char except[]);
	void Zwischenzeit(unsigned long zeitstempel);
};


struct EventTrigger
{
	char			prop_event_title[16];
	unsigned long	prop_threshhold_between;					// millisecond
	unsigned short	prop_clicks;								// Times
	unsigned long	prop_continuous_hold;						// millisecond
	short			prop_tolerance;								// millisecond
	bool			prop_is_first_click;						// event if false or true
	bool			prop_is_shutoff_click;						// event if false or true

	// Validate
	bool			vali_is_triggered_				= false;	// result1 = true if triggered
	bool			vali_is_failed_					= false;	// result1 = true if triggered
	unsigned long	vali_triggered_time				= 0;
	unsigned short	validated_clicks_				= 0;		// Times
	unsigned short	validated_hold_					= 0;		// Times
	unsigned short	validated_press_				= 0;		// Times
	
};

#endif