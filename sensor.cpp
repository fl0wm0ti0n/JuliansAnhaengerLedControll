
// @file           sensor.cpp
// @author         flow@p0cki.at
// @date           01.2017
// @brief          baseclass of all sensors

#include "sensor.h"
#include "constants.h"

sensor::sensor(t_sensor_type t, char n[], unsigned short p)
	:sensortype_(t), sensorname_(n), sensorvalue_(0), sensorpin_(p)
{
	sensor::setPin(sensorpin_);
}

sensor::~sensor()
= default;
