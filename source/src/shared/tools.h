// generic tools for any C++ program

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

#define MAXSTRLEN 260
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

#define defvformatstring(d,last,fmt) string d; { va_list ap; va_start(ap, last); vformatstring(d, fmt, ap); va_end(ap); }

const char *timestring(bool local, const char *fmt = "%Y%m%d_%H.%M.%S"); // sortable time for filenames

#endif
