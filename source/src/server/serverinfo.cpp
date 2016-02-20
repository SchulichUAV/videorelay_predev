// Server info sockets
#include "../shared/pch.h"
#include "server.h"
//#include "serverinfo.h"

namespace serverinfo
{

ENetAddress serveraddress = { ENET_HOST_ANY, ENET_PORT_ANY };

ENetSocket pongsock = ENET_SOCKET_NULL, lansock = ENET_SOCKET_NULL;

void process()
{
    static ENetSocketSet sockset;
    ENET_SOCKETSET_EMPTY(sockset);
    ENetSocket maxsock = pongsock;
    ENET_SOCKETSET_ADD(sockset, pongsock);
    if (lansock != ENET_SOCKET_NULL)
    {
        maxsock = max(maxsock, lansock);
        ENET_SOCKETSET_ADD(sockset, lansock);
    }
    if (enet_socketset_select(maxsock, &sockset, NULL, 0) <= 0) return;

    // reply to all server info requests (just repeat up to 4 bytes)

    static uchar data[1024];
    ENetBuffer buf;
    ENetAddress addr;
    buf.data = data;

    loopi(2)
    {
        ENetSocket sock = i ? lansock : pongsock;
        if (sock == ENET_SOCKET_NULL || !ENET_SOCKETSET_CHECK(sockset, sock)) continue;

        buf.dataLength = sizeof(data);
        int len = enet_socket_receive(sock, &addr, &buf, 1);
        if (len < 0) continue;

        buf.dataLength = min(len, 4);
        enet_socket_send(pongsock, &addr, &buf, 1);
        //*infosend += (int)buf.dataLength;
    }
}

void init(const char *ip, enet_uint16 infoport, enet_uint16 lanport)
{
    ENetAddress address = { ENET_HOST_ANY, infoport };
    if (*ip)
    {
        if (enet_address_set_host(&address, ip)<0) logline(LOGL_WARNING, "server ip not resolved");
        else serveraddress.host = address.host;
    }
    pongsock = enet_socket_create(ENET_SOCKET_TYPE_DATAGRAM);
    if (pongsock != ENET_SOCKET_NULL && enet_socket_bind(pongsock, &address) < 0)
    {
        enet_socket_destroy(pongsock);
        pongsock = ENET_SOCKET_NULL;
    }
    if (pongsock == ENET_SOCKET_NULL) fatal("could not create server info socket")
    else enet_socket_set_option(pongsock, ENET_SOCKOPT_NONBLOCK, 1);
    address.port = lanport;
    lansock = enet_socket_create(ENET_SOCKET_TYPE_DATAGRAM);
    if (lansock != ENET_SOCKET_NULL && (enet_socket_set_option(lansock, ENET_SOCKOPT_REUSEADDR, 1) < 0 || enet_socket_bind(lansock, &address) < 0))
    {
        enet_socket_destroy(lansock);
        lansock = ENET_SOCKET_NULL;
    }
    if (lansock == ENET_SOCKET_NULL) logline(LOGL_WARNING, "could not create LAN server info socket");
    else enet_socket_set_option(lansock, ENET_SOCKOPT_NONBLOCK, 1);
}

}