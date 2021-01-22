
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

	logger_g_->LnWriteLog(F("Call - ram_detail"), extremedebug);
	logger_g_->LnWriteLog(F("SRAM area borders ================="), debug);
	logger_g_->LnWriteLog(F(".bss end     = "), debug);
	logger_g_->WriteLog(String((int)&__bss_end), debug);
	logger_g_->LnWriteLog(F("Heap start   = "), debug);
	logger_g_->WriteLog(String((int)&__heap_start), debug);
	logger_g_->LnWriteLog(F("__brkval     = "), debug);
	logger_g_->WriteLog(String((int)__brkval), debug);
	logger_g_->LnWriteLog(F("Stack pointer= "), debug);
	logger_g_->WriteLog(String((int)SP), debug);
	logger_g_->LnWriteLog(F("Free memory  = "), debug);
	if (__brkval == 0x00)
	{
		logger_g_->WriteLog(String(SP - (int)&__bss_end), debug);
	}
	else
	{
		logger_g_->WriteLog(String(SP - (int)__brkval), debug);
	}
}

static void free_mem()
{
	logger* logger_g_ = logger::GetInstance();
	
	logger_g_->WriteLog(F(", Freemem: "), debug);
	logger_g_->WriteLog(String(free_memory()), debug);
}

#endif
