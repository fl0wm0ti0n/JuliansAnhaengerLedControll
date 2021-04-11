// rgbController.h

#ifndef _RGBLED_h
#define _RGBLED_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#include "actor.h"

class RGBLed : actor
{

public:
	RGBLed(short red, short green, short blue, bool common, char name[]);
	~RGBLed();
	void on(short red, short green, short blue, short brightness);
	void off();
	void nextcolor();
	void nextbrightness();

	void brightness(short rgb[3], short brightness);
	void brightness(short red, short green, short blue, short brightness);

	void flash(short rgb[3], short duration);
	void flash(short rgb[3], short onDuration, short duration);

	void flash(short red, short green, short blue, short duration);
	void flash(short red, short green, short blue, short onDuration, short duration);

	void setColor(short rgb[3]);
	void setColor(short red, short green, short blue);

	void fadeOut(short rgb[3], short steps, short duration);
	void fadeOut(short red, short green, short blue, short steps, short duration);

	void fadeIn(short rgb[3], short steps, short duration);
	void fadeIn(short red, short green, short blue, short steps, short duration);

	static unsigned short RED[3];
	static unsigned short GREEN[3];
	static unsigned short BLUE[3];
	static unsigned short MAGENTA[3];
	static unsigned short CYAN[3];
	static unsigned short YELLOW[3];
	static unsigned short WHITE[3];

	static bool COMMON_ANODE;
	static bool COMMON_CATHODE;


private:
	short common_, red_, green_, blue_, brightness_;
	void color(short red, short green, short blue);
	void blink(short red, short green, short blue, short onDuration, short duration);
	void intensity(short red, short green, short blue, short brightness);
	void fade(short red, short green, short blue, short steps, short duration, bool out);
	void fade(short red, short green, short blue, short steps, short duration, short value);

	unsigned short colorwheel_colors_[7][3];
	unsigned short colorwheel_position_ = 0;
	const unsigned short colorwheel_max_ = 7;
	unsigned short brightnesswheel_position_ = 0;
	const unsigned short brightnesswheel_max_ = 255;
};
#endif