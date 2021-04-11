// @file           actor.cpp
// @author         flow@p0cki.net
// @date           01.2017
// @brief          baseclass of all actors

#include "actor.h"

actor::actor(t_actor_type t, char n[], int p)
	:m_iType(t), m_cName(n), m_iValue(0), m_iPin1(p)
{
	pinMode(m_iPin1, OUTPUT);
}

actor::actor(t_actor_type t, char n[], int p1, int p2, int p3)
	: m_iType(t), m_cName(n), m_iValue(0), m_iPin1(p1), m_iPin2(p2), m_iPin3(p3)
{
	pinMode(m_iPin1, OUTPUT);
	pinMode(m_iPin2, OUTPUT);
	pinMode(m_iPin3, OUTPUT);
}

actor::~actor()
{

}

bool actor::setValue(int v)
{
	m_iValue = v;
	return true;
}