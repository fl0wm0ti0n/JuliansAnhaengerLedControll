
#ifndef _MEMORYINFO_h
#define _MEMORYINFO_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include "logger.h"

#ifdef __arm__
// should use uinstd.h to define sbrk but Due causes a conflict
extern "C" char* sbrk(int incr);
#else  // __ARM__
extern char* __brkval;
extern unsigned int __bss_end;
extern unsigned int __heap_start;
#endif  // __arm__

static int free_memory()
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

static void ram_detail()
{
	logger* logger_g_ = logger::GetInstance();

	static const char* const buffer1 PROGMEM = "------- RAM area borders --------";
	logger_g_->LnWriteLog(buffer1, debug);
	static const char* const buffer2 PROGMEM = ".bss end     = ";
	logger_g_->LnWriteLog(buffer2, debug);
	logger_g_->WriteLog((int)&__bss_end, debug);
	static const char* const buffer3 PROGMEM = "Heap start   = ";
	logger_g_->LnWriteLog(buffer3, debug);
	logger_g_->WriteLog((int)&__heap_start, debug);
	static const char* const buffer4 PROGMEM = "__brkval     = ";
	logger_g_->LnWriteLog(buffer4, debug);
	logger_g_->WriteLog((int)__brkval, debug);
	static const char* const buffer5 PROGMEM = "Stack pointer= ";
	logger_g_->LnWriteLog(buffer5, debug);
	logger_g_->WriteLog((int)SP, debug);
	static const char* const buffer6 PROGMEM = "Free memory  = ";
	logger_g_->LnWriteLog(buffer6, debug);
	if (__brkval == 0x00)
	{
		logger_g_->WriteLog(SP - (int)&__bss_end, debug);
	}
	else
	{
		logger_g_->WriteLog(SP - (int)__brkval, debug);
	}
}

static void free_mem()
{
	logger* logger_g_ = logger::GetInstance();

	static const char* const buffer6 PROGMEM = "Freemem: ";
	logger_g_->LnWriteLog(buffer6, debug);
	logger_g_->WriteLog(free_memory(), debug);
}

#endif
