// server-side logging
// but everything goes to stderr or stdout
#include "../shared/pch.h"
#include "server.h"

namespace
{
static const char *levelprefix[] = { "", "", "", "WARNING: ", "ERROR: " };
static const char *levelname[] = { "DEBUG", "VERBOSE", "INFO", "WARNING", "ERROR", "DISABLED" };
static int consolethreshold = -1; // LOGL_INFO;
static bool timestamp = false;
}

bool initlogging(bool logtimestamp)
{
    timestamp = logtimestamp;
    return true;
}

bool logline(loglevel level, const char *msg, ...)
{
    if (level < 0 || level >= LOGL_NUM) return false;

    defvformatstring(str_formatted, msg, msg);

    const char *ts = timestamp ? timestring(true, "%b %d %Y %H:%M:%S ") : "";
    char *p, *line = str_formatted;
    do
    {
        // break into lines
        if ((p = strchr(line, '\n'))) *p = '\0';
        // log to console
        if (consolethreshold <= level)
        {
            // is puts faster?
            fprintf(stdout, "%s%s%s\n", ts, levelprefix[level], line);
            /*
            if (ts[0])
                fputs(ts, stdout);
            if (levelprefix[level])
                fputs(levelprefix[level], stdout);
            fputs(line, stdout);
            putc('\n', stdout);
            */
        }
        // log to file?
        // log to syslog?
        line = p + 1;
    } while (p);
    if (consolethreshold <= level) fflush(stdout);
    // flush file?
    return consolethreshold <= level;
}
