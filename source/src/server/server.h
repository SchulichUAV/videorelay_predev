#ifndef SERVER_H
#define SERVER_H

#include "serverinfo.h"

struct servercommandline
{
    int serverport = DEFAULT_SERV_PORT;
    const char *ip = "";
    int maxclients = 128;
    int uprate = 0;
    bool logtimestamp = true;

    bool checkarg(const char *arg)
    {
        if (arg[0] != '-' || arg[1] == '\0') return false;

        const char *a = arg + 2 + strspn(arg + 2, " ");
        int ai = atoi(a);

        switch (arg[1])
        {
            case '-':
                if (!strncmp(arg, "--nop", 5))
                {
                    // template for long arguments
                }
                else return false;
                break;
            case 'u': uprate = ai; break;
            case 'f': if (ai > 0 && ai < 65536) serverport = ai; break;
            case 'i': ip = a; break;
            case 'T': logtimestamp = true; break;
            case 't': logtimestamp = false; break;
            case 'c':
                maxclients = clamp(0, ai, MAXCLIENTS);
                break;
            default: return false;
        }
        return false;
    }
};

enum client_type_t
{
    ST_EMPTY = 0,
    ST_IP,
};

struct client
{
    int clientnum;
    client_type_t type = ST_EMPTY;
    ENetPeer *peer = NULL;
    string hostname;
    int connectmillis = 0;
    bool connected = false;

    client() : client(-1) {}
    client(int clientnum) : clientnum(clientnum)
    {
        hostname[0] = '\0';
    }
};

enum loglevel
{
    LOGL_DEBUG = 0,
    LOGL_VERBOSE,
    LOGL_INFO,
    LOGL_WARNING,
    LOGL_ERROR,
    LOGL_NUM
};
bool initlogging(bool logtimestamp);
bool logline(loglevel level, const char *msg, ...);

#endif