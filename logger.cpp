// @file      analogIn.cpp
// @author    flow@p0cki.net
// @date      08.2019
// @brief     baseclass of logging

#include "logger.h"
#include "Streaming.h"

logger* logger::singleton_ = nullptr;;

/**
 * Static methods should be defined outside the class.
 */
logger* logger::GetInstance(t_log_level t, t_log_target g, String n)
{
	/**
	 * This is a safer way to create an instance. instance = new Singleton is
	 * dangeruous in case two instance threads wants to access at the same time
	 */
	//DEBUG_PRINTLN(F("_called GetInstance"));

	if (singleton_ == nullptr) {
		singleton_ = new logger(t, g, n);
	}
	return singleton_;
}

logger* logger::GetInstance()
{
	//DEBUG_PRINTLN(F("_called default GetInstance"));
	return singleton_;
}

void logger::LnWriteLog(String text, t_log_level llevel)
{
	bool newline = false;
	//DEBUG_PRINTLN(F("_called WriteLog"));
	if (GetLogLevel() != disabled)
	{
		HandleLog(GetActualTimestamp(), text, llevel, newline);
	}
}

void logger::WriteLogLn(String text, t_log_level llevel)
{
	bool newline = true;
	//DEBUG_PRINTLN(F("_called WriteLog"));
	if (GetLogLevel() != disabled)
	{
	HandleLog(F("0"), text, llevel, newline);
	}
}

void logger::WriteLog(String text, t_log_level llevel)
{
	bool newline = false;
	//DEBUG_PRINTLN(F("_called WriteLog"));
	if (GetLogLevel() != disabled)
	{
		HandleLog(F("0"), text, llevel, newline);
	}
}

String logger::GetActualTimestamp()
{
	//DEBUG_PRINTLN(F("_called GetActualTimestamp"));
	i_sekunde_ = millis() / 1000;
	if (i_sekunde_ >= 3600)
	{
		i_stunde_ = 0;
		while (i_sekunde_ >= 3600)
		{
			i_stunde_++;
			i_sekunde_ = i_sekunde_ - 3600;
		}
	}

	if (i_sekunde_ >= 60)
	{
		i_minute_ = 0;
		while (i_sekunde_ >= 60)
		{
			i_minute_++;
			i_sekunde_ = i_sekunde_ - 60;
		}
	}

	c_stunde_ = String(i_stunde_);
	c_minute_ = String(i_minute_);
	c_sekunde_ = String(i_sekunde_);

	if (c_stunde_.length()	== 1) { c_stunde_		= "0" + c_stunde_; }
	if (c_minute_.length()	== 1) { c_minute_		= "0" + c_minute_; }
	if (c_sekunde_.length()	== 1) { c_sekunde_	= "0" + c_sekunde_; }

	c_timestamp_ = c_stunde_ + ":" + c_minute_ + ":" + c_sekunde_;

	return c_timestamp_;
}

String logger::GetLogArchive(t_log_level llevel)
{
	//DEBUG_PRINTLN(F("_called GetLogArchive"));
	String output;
	if (GetLogLevel() != disabled)
	{
		if (i_log_target_ == archive)
		{
			if (i_log_lvl_ == llevel)
			{
				//noch nicht implementiert!
				output = F("Funktion wurde noch nicht implementiert und benötigt ein Hardwaremodul!");
			}
		}
		else
		{
			output = F("Das Archivieren der Protokolle ist deaktiviert!");
		}
		output = F("Das Logging wurde deaktiviert!");
	}
	return output;
}

void logger::HandleLog(String time, String text, t_log_level llevel, bool newline)
{
	//DEBUG_PRINTLN(F("_called HandleLog"));
	if (i_log_lvl_ >= llevel)
	{
		switch (i_log_target_)
		{
		case 0:
			//Serial.println(String(time) + " : " + String(c_enum_lvl_[llevel]) + " : " + String(text));
			/*Serial.print(String(time));
			Serial.print(" : ");
			Serial.print(String(c_enum_lvl_[llevel]));
			Serial.print(" : ");
			Serial.println(String(text));*/ 

			if (newline)
			{
				Serial << text << endl;
				break;
			}
			if (time != F("0"))
			{
				Serial << endl << time << " : " << c_enum_lvl_[llevel] << " : " << text;
				break;
			}
			Serial  << text;
			break;
		case 1:
			// Netzwerk Stream - noch nicht implementiert!
			break;
		case 2:
			// Archivdata - noch nicht implementiert!
			break;
		default:
			//Serial.println(String(time) + " : " + String(c_enum_lvl_[llevel]) + " : " + String(text));
			if (newline)
			{
				Serial << text << endl;
				break;
			}
			if (time != F("0"))
			{
				Serial << endl << time << " : " << c_enum_lvl_[llevel] << " : " << text;
				break;
			}
			Serial << text;
			break;
		}
	}
}