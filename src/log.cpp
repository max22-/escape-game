#include "fe_repl.h"
#include <stdarg.h>
#include <stdio.h>

bool log_enabled = false;

void _log(const char *fmt, ...) {
  char buffer[256];
  va_list args;
  if (log_enabled) {
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    wifi_client_write(buffer);
    va_end(args);
  }
}