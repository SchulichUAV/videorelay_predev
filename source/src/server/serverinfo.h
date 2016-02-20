#ifndef __SERVERINFO_H__
#define __SERVERINFO_H__

namespace serverinfo
{

void process();
void init(const char *ip, enet_uint16 infoport, enet_uint16 lanport = SERVINFO_PORT_LAN);

}

#endif