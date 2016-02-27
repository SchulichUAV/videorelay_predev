// implementation for generic tools

#include "pch.h"

// A large portion of this code is
// from Cube, AssaultCube, and AssaultCube Reloaded
// released under the Cube license.

const char *timestring(bool local, const char *fmt)
{
    static string asciitime;
    time_t t = time(NULL);
    struct tm * timeinfo = local ? localtime(&t) : gmtime(&t);
    strftime(asciitime, sizeof(string) - 1, fmt, timeinfo);
    return asciitime;
}

#define N (624)
#define M (397)
#define K (0x9908B0DFU)

static uint state[N];
static int next = N;

void seedMT(uint seed)
{
    state[0] = seed;
    for (uint i = 1; i < N; i++)
        state[i] = seed = 1812433253U * (seed ^ (seed >> 30)) + i;
    next = 0;
}

uint randomMT()
{
    int cur = next;
    if (++next >= N)
    {
        if (next > N) { seedMT(5489U + (uint)time(NULL)); cur = 0; next = 1; }
        else next = 0;
    }
    uint y = (state[cur] & 0x80000000U) | (state[next] & 0x7FFFFFFFU);
    state[cur] = y = state[cur < N - M ? cur + M : cur + M - N] ^ (y >> 1) ^ ((uint)(-((int)y & 1)) & K);
    y ^= (y >> 11);
    y ^= (y << 7) & 0x9D2C5680U;
    y ^= (y << 15) & 0xEFC60000U;
    y ^= (y >> 18);
    return y;
}

const char *iptoa(const enet_uint32 ip)
{
    static string s[2];
    static int buf = 0;
    if (++buf == 2)
        buf = 0;
    formatstring(s[buf])("%d.%d.%d.%d", (ip >> 24) & 255, (ip >> 16) & 255, (ip >> 8) & 255, ip & 255);
    return s[buf];
}

char *concatformatstring(char *d, const char *s, ...)
{
    static defvformatstring(temp, s, s);
    return concatstring(d, temp);
}
