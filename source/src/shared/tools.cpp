#include "pch.h"

const char *timestring(bool local, const char *fmt)
{
    static string asciitime;
    time_t t = time(NULL);
    struct tm * timeinfo = local ? localtime(&t) : gmtime(&t);
    strftime(asciitime, sizeof(string) - 1, fmt, timeinfo);
    return asciitime;
}
