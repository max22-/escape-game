#ifndef LOG_H
#define LOG_H

#define LOG(...) _log(__VA_ARGS__)
void _log(const char *fmt, ...);
extern bool log_enabled;

#endif