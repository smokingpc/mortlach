#pragma once
#include <stdarg.h>

//================================================================
// DebugPrint functions. These functions use DbgPrintEx()
//================================================================
#define LOG_LEVEL_BASE      0x00000020
#define LOG_LEVEL_DEBUG     (LOG_LEVEL_BASE + 0)
#define LOG_LEVEL_INFO      (LOG_LEVEL_BASE + 1)
#define LOG_LEVEL_WARN      (LOG_LEVEL_BASE + 2)
#define LOG_LEVEL_ERROR     (LOG_LEVEL_BASE + 4)
__inline void PrintLog(
    _In_ ULONG level,
    _In_ const char* msg,
    _In_ va_list arglist)
{
    vDbgPrintEx(DPFLTR_IHVDRIVER_ID, level, msg, arglist);
}

__inline void PrintError(_In_ const char* msg, ...)
{
    va_list arglist;
    va_start(arglist, msg);
    PrintLog(LOG_LEVEL_ERROR, msg, arglist);
    va_end(arglist);
}

__inline void PrintWarng(_In_ const char* msg, ...)
{
    va_list arglist;
    va_start(arglist, msg);
    PrintLog(LOG_LEVEL_WARN, msg, arglist);
    va_end(arglist);
}

__inline void PrintInfo(_In_ const char* msg, ...)
{
    va_list arglist;
    va_start(arglist, msg);
    PrintLog(LOG_LEVEL_INFO, msg, arglist);
    va_end(arglist);
}

#if defined(DBG)
__inline void PrintDebug(_In_ const char* msg, ...)
{
    va_list arglist;
    va_start(arglist, msg);
    PrintLog(LOG_LEVEL_DEBUG, msg, arglist);
    va_end(arglist);
}
#else
#define PrintDebug(msg, ...)
#endif

//================================================================
// class definition to automatically print log messages.
//================================================================

// print function IN/OUT log automatically.
// always print function IN/OUT log in debug level.
#define SPC_LOG_PREFIX      "[SPC]"
class CFuncLog {
public:
    CFuncLog(const char* funcname, const char *prefix = SPC_LOG_PREFIX) {
        FUNCNAME = funcname;
        PREFIX = prefix;
        PrintDebug("%s%s ==> IN\n", PREFIX, FUNCNAME);
    }
    ~CFuncLog() {
        PrintDebug("%s%s <== OUT\n", PREFIX, FUNCNAME);
    }
protected:
    const char *FUNCNAME = NULL;
    const char* PREFIX = NULL;
};

