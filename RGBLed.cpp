// 
// https://github.com/wilmouths/RGBLed/blob/master/src/RGBLed.cpp
//

#include "RGBLed.h"

unsigned short RGBLed::RED[3] = { 255, 0, 0 };
unsigned short RGBLed::GREEN[3] = { 0, 255, 0 };
unsigned short RGBLed::BLUE[3] = { 0, 0, 255 };
unsigned short RGBLed::MAGENTA[3] = { 255, 0, 255 };
unsigned short RGBLed::CYAN[3] = { 0, 255, 255 };
unsigned short RGBLed::YELLOW[3] = { 255, 255, 0 };
unsigned short RGBLed::WHITE[3] = { 255, 255, 255 };

bool RGBLed::COMMON_ANODE = true;
bool RGBLed::COMMON_CATHODE = false;

RGBLed::~RGBLed()
= default;

RGBLed::RGBLed(short redp, short greenp, short bluep, bool common, char name[])
	: actor(RGB_act, name, redp, greenp, bluep)
{
	common_ = common;

	for (short j = 0; j <= 2; ++j)
	{
		colorwheel_colors_[0][j] = { RED[j] };
	}
	for (short j = 0; j <= 2; ++j)
	{
		colorwheel_colors_[1][j] = { GREEN[j] };
	}
	for (short j = 0; j <= 2; ++j)
	{
		colorwheel_colors_[2][j] = { BLUE[j] };
	}
	for (short j = 0; j <= 2; ++j)
	{
		colorwheel_colors_[3][j] = { MAGENTA[j] };
	}
	for (short j = 0; j <= 2; ++j)
	{
		colorwheel_colors_[4][j] = { CYAN[j] };
	}
	for (short j = 0; j <= 2; ++j)
	{
		colorwheel_colors_[5][j] = { YELLOW[j] };
	}
	for (short j = 0; j <= 2; ++j)
	{
		colorwheel_colors_[6][j] = { WHITE[j] };
	}
	
}

void RGBLed::on(short red, short green, short blue, short brightness)
{
	if (red_ == 0 && green_ == 0 && blue_ == 0)
	{
		intensity(red, green, blue, brightness);
	}
	else
	{
		if (brightness_ != 0)
		{
			intensity(red_, green_, blue_, brightness_);
		}
		else
		{
			intensity(red_, green_, blue_, brightness);
		}
	}
}

void RGBLed::off()
{
	color(0, 0, 0);
}

void RGBLed::nextcolor()
{
	if (colorwheel_position_ < colorwheel_max_)
	{
		colorwheel_position_++;
	}
	else
	{
		colorwheel_position_ = 0;
	}
	intensity(colorwheel_colors_[colorwheel_position_][0], colorwheel_colors_[colorwheel_position_][1], colorwheel_colors_[colorwheel_position_][2],brightness_);
}

void RGBLed::nextbrightness()
{
	if (brightnesswheel_position_ < brightnesswheel_max_)
	{
		brightnesswheel_position_ += 51;
	}
	else
	{
		brightnesswheel_position_ = 0;
	}
	intensity(red_, green_, blue_, brightnesswheel_position_);
}

void RGBLed::brightness(short rgb[3], short brightness)
{
	intensity(rgb[0], rgb[1], rgb[2], brightness);
}

void RGBLed::brightness(short red, short green, short blue, short brightness)
{
	intensity(red, green, blue, brightness);
}

void RGBLed::intensity(short red, short green, short blue, short brightness)
{
	red = (red * brightness) / 100;
	green = (green * brightness) / 100;
	blue = (blue * brightness) / 100;
	color(red, green, blue);
}

void RGBLed::flash(short rgb[3], short duration)
{
	blink(rgb[0], rgb[1], rgb[2], duration, duration);
}

void RGBLed::flash(short rgb[3], short onDuration, short duration)
{
	blink(rgb[0], rgb[1], rgb[2], onDuration, duration);
}

void RGBLed::flash(short red, short green, short blue, short duration)
{
	blink(red, green, blue, duration, duration);
}

void RGBLed::flash(short red, short green, short blue, short onDuration, short duration)
{
	blink(red, green, blue, onDuration, duration);
}

void RGBLed::blink(short red, short green, short blue, short onDuration, short duration)
{
	color(red, green, blue);
	delay(onDuration);
	off();
	delay(duration);
}

void RGBLed::setColor(short rgb[3])
{
	color(rgb[0], rgb[1], rgb[2]);
}

void RGBLed::setColor(short red, short green, short blue)
{
	color(red, green, blue);
}

void RGBLed::color(short red, short green, short blue)
{
	red_ = red;
	green_ = green;
	blue_ = blue;
	
	if (common_ == COMMON_ANODE) {
		analogWrite(m_iPin1, ~red);
		analogWrite(m_iPin2, ~green);
		analogWrite(m_iPin3, ~blue);
	}
	else {
		analogWrite(m_iPin1, red);
		analogWrite(m_iPin2, green);
		analogWrite(m_iPin3, blue);
	}
}

void RGBLed::fadeOut(short rgb[3], short steps, short duration)
{
	fade(rgb[0], rgb[1], rgb[2], steps, duration, true);
}

void RGBLed::fadeOut(short red, short green, short blue, short steps, short duration)
{
	fade(red, green, blue, steps, duration, true);
}

void RGBLed::fadeIn(short rgb[3], short steps, short duration)
{
	fade(rgb[0], rgb[1], rgb[2], steps, duration, false);
}

void RGBLed::fadeIn(short red, short green, short blue, short steps, short duration)
{
	fade(red, green, blue, steps, duration, false);
}

void RGBLed::fade(short red, short green, short blue, short steps, short duration, bool out)
{
	red_ = red;
	green_ = green;
	blue_ = blue;
	if (out) {
		for (short i = 255; i >= 0; i -= steps) {
			fade(red, green, blue, steps, duration, i);
		}
	}
	else {
		for (short i = 0; i <= 255; i += steps) {
			fade(red, green, blue, steps, duration, i);
		}
	}
}

void RGBLed::fade(short red, short green, short blue, short steps, short duration, short value)
{
	red_ = red;
	green_ = green;
	blue_ = blue;
	float brightness = float(value) / 255.f;
	if (red > 0 && red <= 255) analogWrite(m_iPin1, red * brightness);
	if (green > 0 && green <= 255) analogWrite(m_iPin2, green * brightness);
	if (blue > 0 && blue <= 255) analogWrite(m_iPin3, blue * brightness);

	delay((unsigned long)(duration / steps));
}