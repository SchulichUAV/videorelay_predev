// generic tools for any C++ program

// A large portion of this code is
// from Cube, AssaultCube, and AssaultCube Reloaded
// released under the Cube license.

#ifndef _TOOLS_H
#define _TOOLS_H

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;

#define clamp(a,b,c) (max(b, min(a, c)))

#define loop(v,m) for(int v = 0; v<int(m); v++)
#define loopi(m) loop(i,m)
#define loopj(m) loop(j,m)
#define loopk(m) loop(k,m)
#define loopl(m) loop(l,m)

// fast safe strings

#define MAXSTRLEN 512
typedef char string[MAXSTRLEN];

inline void vformatstring(char *d, const char *fmt, va_list v, int len = MAXSTRLEN)
{
    vsnprintf(d, len, fmt, v);
    d[len - 1] = '\0';
}
inline char *copystring(char *d, const char *s, size_t len = MAXSTRLEN)
{
    strncpy(d, s, len);
    d[len - 1] = '\0';
    return d;
}

inline char *concatstring(char *d, const char *s, size_t len = MAXSTRLEN)
{
    size_t used = strlen(d);
    return used < len ? copystring(d + used, s, len - used) : d;
}

extern char *concatformatstring(char *d, const char *s, ...);

struct stringformatter
{
    char *buf;
    stringformatter(char *buf) : buf((char *)buf) {}
    void operator()(const char *fmt, ...)
    {
        va_list v;
        va_start(v, fmt);
        vformatstring(buf, fmt, v);
        va_end(v);
    }
};

#define formatstring(d) stringformatter((char *)d)
#define defformatstring(d) string d; formatstring(d)
#define defvformatstring(d,last,fmt) string d; { va_list ap; va_start(ap, last); vformatstring(d, fmt, ap); va_end(ap); }

// just use STL storage containers
#include <vector>

const char *timestring(bool local, const char *fmt = "%Y%m%d_%H.%M.%S"); // sortable time for filenames

// endian swap stuff?

void seedMT(uint seed);
uint randomMT();

const char *iptoa(const enet_uint32 ip);

#define fatal(msg) { \
    fputs("fatal error: " msg "\n", stderr); \
    exit(EXIT_FAILURE); \
}

#endif
