#include "log.h"
#include <stdio.h>
#include <stdarg.h>

using namespace common;

void Log::Warning(const char* fmt, ...)
{
    printf("WARNING:\n");

    va_list	argptr;
    va_start(argptr,fmt);
    printf(fmt, argptr);
    va_end(argptr);

    printf("\n");
}

void Log::Error(const char* fmt, ...)
{
    printf("ERROR:\n");

    va_list	argptr;
    va_start(argptr,fmt);
    printf(fmt, argptr);
    va_end(argptr);

    printf("\n");
}
