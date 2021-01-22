
// @file           home_doggle.cpp
// @author         flow@p0cki.at
// @date           12.2016
// @brief          flip a Value

#include "doggle.h"

int doggle::iflip = false;
int doggle::iflop = false;

doggle::doggle()
{
	iReturnValue = false;
}

doggle::~doggle()
{

}

int doggle::flip(int iValue)
{

	//********************** routine ************************
	if (iValue == HIGH)
	{
		if (!iflop)
		{
			iflop = true;
			iflip = !iflip;

			if (iflip)
			{
				iReturnValue = true;
			}
			else
			{
				iReturnValue = false;
			}
		}
	}
	else
	{
		iflop = false;
	}
	return iReturnValue;
}