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

enum t_log_level
{
	disabled = -1,
	error = 0,
	warning = 1,
	sensordata = 2,
	debug = 3,
	extremedebug = 4,
};

enum t_log_target
{
	serial = 0,
	net = 1,
	archive = 2,
};


class logger
{
	
private:
	// Variablen des Constructors
	t_log_level		i_log_lvl_;
	t_log_target	i_log_target_;
	String			c_name_;

	// Variablen zur Zeitberechnung
	String			c_timestamp_;
	String			c_sekunde_;
	String			c_minute_;
	String			c_stunde_;
	int				i_sekunde_ = 0;
	int				i_minute_ = 0;
	int				i_stunde_ = 0;

	// Variablen zur Loghochzähulung
	unsigned int	i_num_errors;
	unsigned int	i_num_warnings_;
	unsigned int    i_num_sensors_;
	unsigned int    i_num_debugs_;
	unsigned int    i_num_extremedebugs_;

	// Weitere Variablen
	String			c_enum_lvl_[5];
	String			c_log_archive_;
	String			c_log_message_;

	void HandleLog(String time, String text, t_log_level llevel, bool newline);

	/**
	 * The Singleton's constructor should always be private to prevent direct
	 * construction calls with the `new` operator.
	 */
protected:
	logger(t_log_level t, t_log_target g, String n) : i_log_lvl_(t), i_log_target_(g), c_name_(n)
	{
		switch (i_log_target_)
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

		// inizialisiere Array für Loglevel Ausgabe
		c_enum_lvl_[0] = F("error");
		c_enum_lvl_[1] = F("warni");
		c_enum_lvl_[2] = F("senso");
		c_enum_lvl_[3] = F("debug");
		c_enum_lvl_[4] = F("exdeb");
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
	static logger* GetInstance(t_log_level t, t_log_target g, String n);
	static logger* GetInstance();

	void LnWriteLog(String text, t_log_level llevel);
	void WriteLogLn(String text, t_log_level llevel);
	void WriteLog(String text, t_log_level llevel);
	String GetActualTimestamp();
	String GetLogArchive(t_log_level llevel);

	void SetLoglevel(t_log_level m_iLogLvl_)
	{
		this->i_log_lvl_ = m_iLogLvl_;
	}

	void SetLogTarget(t_log_target m_iLogTarget_)
	{
		this->i_log_target_ = m_iLogTarget_;
	}

	t_log_level GetLogLevel()
	{
		return i_log_lvl_;
	}

	t_log_target GetLogTarget()
	{
		return i_log_target_;
	}

	String GetName()
	{
		return c_name_;
	}
};

//class logger
//{
//
//private:
//
//	// Variablen des Constructors
//	t_log_level		i_log_lvl_;
//	t_log_target	i_log_target_;
//	String			c_name_;
//
//	// Variablen zur Zeitberechnung
//	String			c_timestamp_;
//	String			c_sekunde_;
//	String			c_minute_;
//	String			c_stunde_;
//	int				i_sekunde_ = 0;
//	int				i_minute_ = 0;
//	int				i_stunde_ = 0;
//
//	// Variablen zur Loghochzähulung
//	unsigned int	i_num_errors;
//	unsigned int	i_num_warnings_;
//	unsigned int    i_num_sensors_;
//	unsigned int    i_num_debugs_;
//	unsigned int    i_num_extremedebugs_;
//
//	// Weitere Variablen
//	String			c_enum_lvl_[5];
//	String			c_log_archive_;
//	String			c_log_message_;
//
//	void handleLog(String time, String text, t_log_level llevel);
//
//public:
//	logger(t_log_level t, t_log_target g, String n);		// Constructor
//	virtual ~logger();									// Destructor
//
//	void WriteLog(String text, t_log_level llevel);
//	String GetActualTimestamp();
//	String GetLogArchive(t_log_level llevel);
//
//	void SetLoglevel(t_log_level i_log_lvl_)
//	{
//		this->i_log_lvl_ = i_log_lvl_;
//	}
//
//	void SetLogTarget(t_log_target i_log_target_)
//	{
//		this->i_log_target_ = i_log_target_;
//	}
//
//	t_log_level GetLogLevel()
//	{
//		return i_log_lvl_;
//	}
//
//	t_log_target GetLogTarget()
//	{
//		return i_log_target_;
//	}
//
//	String GetName()
//	{
//		return c_name_;
//	}
//};
#endif