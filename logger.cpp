// @file      analogIn.cpp
// @author    flow@p0cki.net
// @date      08.2019
// @brief     baseclass of logging

#include "logger.h"

logger* logger::singleton_ = nullptr;;

/**
 * Static methods should be defined outside the class.
 */
logger* logger::GetInstance(log_level t, log_target g, char n[])
{
	/**
	 * This is a safer way to create an instance. instance = new Singleton is
	 * dangeruous in case two instance threads wants to access at the same time
	 */
	//Serial.println("TEST");
	if (singleton_ == nullptr) {
		singleton_ = new logger(t, g, n);
	}
	return singleton_;
}

logger* logger::GetInstance()
{
	//Serial.println("TEST");
	return singleton_;
}

char* logger::convert_bool_to_char(bool value)
{
	char *varbool = new char[6];
	if (value)
	{
		strcpy(varbool, "true");
	}
	else
	{
		strcpy(varbool, "false");
	}
	return varbool;
}

char* logger::GetActualTimestamp()
{
	sekunde_ = millis() / 1000;
	
	if (sekunde_ >= 3600)
	{
		stunde_ = 0;
		while (sekunde_ >= 3600)
		{
			stunde_++;
			sekunde_ = sekunde_ - 3600;
		}
	}
	
	if (sekunde_ >= 60)
	{
		minute_ = 0;
		while (sekunde_ >= 60)
		{
			minute_++;
			sekunde_ = sekunde_ - 60;
		}
	}

	static char buffer[sizeof(byte) * 8 + 1];
	strcpy(stunde_char_, itoa(stunde_, buffer, 10));
	strcpy(minute_char_, itoa(minute_, buffer, 10));
	strcpy(sekunde_char_, itoa(sekunde_, buffer, 10));

	if (strlen(stunde_char_) == 1) { strcpy(timestamp_char_, "0"); strcat(timestamp_char_, stunde_char_); }
	else { strcpy(timestamp_char_, stunde_char_); }
	strcat(timestamp_char_, ":");
	if (strlen(minute_char_) == 1) { strcat(timestamp_char_, "0"); }
	strcat(timestamp_char_, minute_char_);
	strcat(timestamp_char_, ":");
	if (strlen(sekunde_char_) == 1) { strcat(timestamp_char_, "0"); }
	strcat(timestamp_char_, sekunde_char_);

	return timestamp_char_;
}

char* logger::GetLogArchive(log_level llevel)
{
	char output[256];
	if (GetLogLevel() != disabled)
	{
		if (log_target_ == archive)
		{
			if (log_lvl_ == llevel)
			{
				//noch nicht implementiert!
				static const char* const message PROGMEM = "Funktion wurde noch nicht implementiert und benötigt ein Hardwaremodul!";
				strcpy(output, message);
			}
		}
		else
		{
			static const char* const message PROGMEM = "Das Archivieren der Protokolle ist deaktiviert!";
			strcpy(output, message);
		}
		static const char* const message PROGMEM = "Das Logging wurde deaktiviert!";
		strcpy(output, message);
	}
	return output;
}

int logger::free_memory()
{
	char top;
#ifdef __arm__
	return &top - reinterpret_cast<char*>(sbrk(0));
#elif defined(CORE_TEENSY) || (ARDUINO > 103 && ARDUINO != 151)
	return &top - __brkval;
#else  // __arm__
	return __brkval ? &top - __brkval : &top - __malloc_heap_start;
#endif  // __arm__
}