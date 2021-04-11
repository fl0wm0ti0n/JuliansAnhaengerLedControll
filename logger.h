// @file           logger.h
// @author         flow@p0cki.at
// @date           08.2019
// @brief          baseclass of logging

#ifndef _HOME_LOG_h
#define _HOME_LOG_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include "constants.h"
#include "Streaming.h"

#ifdef __arm__
// should use uinstd.h to define sbrk but Due causes a conflict
extern "C" char* sbrk(int incr);
#else  // __ARM__
extern char* __brkval;
extern unsigned int __bss_end;
extern unsigned int __heap_start;
#endif  // __arm__

enum log_level
{
	disabled = -1,
	error = 0,
	warning = 1,
	sensordata = 2,
	debug = 3,
	extremedebug = 4,
};

enum log_target
{
	serial = 0,
	net = 1,
	archive = 2,
};


class logger
{
	
private:
	// Variablen des Constructors
	log_level		log_lvl_;
	log_target		log_target_;
	char			name_;

	// Variablen zur Zeitberechnung
	char			timestamp_char_[17];
	char			sekunde_char_[3];
	char			minute_char_[3];
	char			stunde_char_[3];
	//char			buffer[sizeof(int) * 8 + 1];
	int				sekunde_ = 0;
	byte			minute_ = 0;
	byte			stunde_ = 0;

	// Variablen zur Loghochzähulung
	unsigned int	num_errors;
	unsigned int	num_warnings_;
	unsigned int    num_sensors_;
	unsigned int    num_debugs_;
	unsigned int    num_extremedebugs_;

	// Weitere Variablen
	char			log_archive_;
	char			log_message_;
	
	const char* const warni PROGMEM = "warni";
	const char* const senso PROGMEM = "senso";
	const char* const error PROGMEM = "error";
	const char* const debug PROGMEM = "debug";
	const char* const exdeb PROGMEM = "exdeb";

	// Initialisiere die Tabelle von Strings
	const char* const loglevel_table[5] PROGMEM = { error, warni, senso, debug, exdeb };

	void HandleLog(char time[], char text[], log_level llevel, bool newline);
	
	/**
	 * The Singleton's constructor should always be private to prevent direct
	 * construction calls with the `new` operator.
	 */
protected:
	logger(log_level t, log_target g, char n[]) : log_lvl_(t), log_target_(g), name_(*n)
	{
		switch (log_target_)
		{
		case 0:
			Serial.begin(DEFAULT_BAUTRATE);
			break;
		case 1:
			// Netzwerk init - noch nicht implementiert!
			break;
		case 2:
			// Archiv init - noch nicht implementiert!
			break;
		default:
			break;
		}	
	}

	static logger* singleton_;

public:

	/**
	 * Singletons should not be cloneable.
	 */
	logger(logger& other) = delete;
	/**
	 * Singletons should not be assignable.
	 */
	void operator=(const logger&) = delete;

	/**
	 * This is the static method that controls the access to the singleton
	 * instance. On the first Run, it creates a singleton object and places it
	 * into the static field. On subsequent runs, it returns the client existing
	 * object stored in the static field.
	 */
	static logger* GetInstance(log_level t, log_target g, char n[]);
	static logger* GetInstance();

	template <typename T>
	void LnWriteLog(T value, log_level llevel);
	template <typename T>
	void WriteLogLn(T value, log_level llevel);
	template <typename T>
	void WriteLog(T value, log_level llevel);

	template<typename T>
	void FinalHandleLog(char time[], T text, log_level llevel, bool newline);
	
	char* convert_bool_to_char(bool value);
	char* GetActualTimestamp();
	char* GetLogArchive(log_level llevel);
	int free_memory();

	void SetLoglevel(log_level m_iLogLvl_)
	{
		this->log_lvl_ = m_iLogLvl_;
	}

	void SetLogTarget(log_target m_iLogTarget_)
	{
		this->log_target_ = m_iLogTarget_;
	}

	log_level GetLogLevel() 
	{
		return log_lvl_;
	}

	log_target GetLogTarget()
	{
		return log_target_;
	}

	char GetName()
	{
		return name_;
	}
};

template<typename T>
void CovertToChar(char*& to, T from) { strcpy(to, from);}

template<>
inline void CovertToChar<bool>(char*& to, bool from) { if (from) { strcpy(to, "true"); } else { strcpy(to, "false"); } }

template<>
inline void CovertToChar<int>(char*& to, int from) { itoa(from, to, 10); }

template<>
inline void CovertToChar<unsigned int>(char*& to, unsigned int from) { ultoa(from, to, 10); }

template<>
inline void CovertToChar<short>(char*& to, short from) { itoa(from, to, 10); }

template<>
inline void CovertToChar<unsigned short>(char*& to, unsigned short from) { ultoa(from, to, 10); }

template<>
inline void CovertToChar<long>(char*& to, long from) { ltoa(from, to, 10); }

template<>
inline void CovertToChar<unsigned long>(char*& to, unsigned long from) { ultoa(from, to, 10); }

template<>
inline void CovertToChar<float>(char*& to, float from) { snprintf(to, sizeof to, "%f", from); }

template<>
inline void CovertToChar<double>(char*& to, double from) { snprintf(to, sizeof to, "%f", from); }

template<>
inline void CovertToChar<char*>(char*& to, char from[]) { strcpy(to, from); }


template <typename T>
void logger::LnWriteLog(T value, log_level llevel)
{
//#ifdef MEMINFO_
//	Serial << endl << "Free Memory: " << free_memory() << endl;
//#endif
	if (GetLogLevel() != disabled)
	{
		//char* creturn = GetTypeofAndCovertToChar(value);
		//char buffering[sizeof(creturn) + 2];
		//strcpy(buffering, creturn);
		//memset(creturn, '\0', 1);
		//delete[] creturn;
		FinalHandleLog(GetActualTimestamp(), value, llevel, false);
		//delete[] creturn;
	}
}

template <typename T>
void logger::WriteLogLn(T value, log_level llevel)
{
//#ifdef MEMINFO_
//	Serial << endl << "Free Memory: " << free_memory() << endl;
//#endif
	if (GetLogLevel() != disabled)
	{
		//char* creturn = GetTypeofAndCovertToChar(value);
		//char buffering[sizeof(creturn) + 2];
		//strcpy(buffering, creturn);
		//memset(creturn, '\0', 1);
		//delete[] creturn;
		FinalHandleLog(NULL, value, llevel, true);
		//delete[] creturn;
	}
}

template <typename T>
void logger::WriteLog(T value, log_level llevel)
{
//#ifdef MEMINFO_
//	Serial << endl << "Free Memory: " << free_memory() << endl;
//#endif
	if (GetLogLevel() != disabled)
	{
		FinalHandleLog(NULL, value, llevel, false);
	}
}

template <typename T>
void logger::FinalHandleLog(char time[], T text, log_level llevel, bool newline)
{
	char* convertedc = new char[sizeof(T) * 8 + 1];
	CovertToChar(convertedc, text);
	if (log_lvl_ >= llevel)
	{
		switch (log_target_)
		{
		case 0:
			if (newline)
			{
				Serial << convertedc << endl;
				delete[] convertedc;
				break;
			}
			if (time != NULL)
			{
				Serial << endl << time << " : " << loglevel_table[llevel] << " : " << convertedc;
				delete[] convertedc;
				break;
			}
			Serial << convertedc;
			delete[] convertedc;
			break;
		case 1:
			// Netzwerk Stream - noch nicht implementiert!
			break;
		case 2:
			// Archivdata - noch nicht implementiert!
			break;
		default:
			if (newline)
			{
				Serial << convertedc << endl;
				delete[] convertedc;
				break;
			}
			if (time != NULL)
			{
				Serial << endl << time << " : " << loglevel_table[llevel] << " : " << convertedc;
				delete[] convertedc;
				break;
			}
			Serial << convertedc;
			delete[] convertedc;
			break;
		}
	}
}
#endif