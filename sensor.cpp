
// @file           sensor.cpp
// @author         flow@p0cki.at
// @date           01.2017
// @brief          baseclass of all sensors

#include "sensor.h"
#include "constants.h"

sensor::sensor(t_sensor_type t, char n[], int p)
	:m_iType(t), m_cName(n), m_iValue(0), m_iPin(p)
{
	sensor::setPin(m_iPin);
}

sensor::~sensor()
= default;
