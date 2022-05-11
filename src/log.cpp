#include <stdio.h>
#include <stdarg.h>
#include "fe_repl.h"

bool log_enabled = false;

void _log(const char *fmt, ...)
{
    char buffer[256];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    wifi_client_write(buffer);
    va_end(args);
}