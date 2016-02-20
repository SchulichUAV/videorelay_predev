#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

#define PROTOCOL_VERSION 1
#define MAXCLIENTS 256
#define DEFAULT_SERV_PORT 1337
#define SERVINFO_PORT_LAN 1339

enum chan_t
{
    CHAN_TEXT = 0,
    CHAN_VIDEO,
    CHAN_INIT,
    CHAN_NUM,
};

#endif