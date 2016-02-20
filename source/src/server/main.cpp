// Main entry point of server application
#include "../shared/pch.h"
#include "server.h"

servercommandline scl;
ENetHost *serverhost;
int servmillis = 0;
client clients[MAXCLIENTS];

client &addclient(client_type_t type = ST_IP)
{
    loopi(MAXCLIENTS)
    {
        if (clients[i].type == ST_EMPTY)
        {
            clients[i].clientnum = i;
            return clients[i];
        }
    }

    static client dummy(MAXCLIENTS);
    return dummy;
}

void process(ENetPacket *packet, client &c, chan_t channelID)
{
    if (!c.connected)
    {
        // TODO: check channel
        c.connected = true;
    }
}

void disconnect_client(client &c, int reason = -1)
{
    logline(LOGL_INFO, "[%s] disconnected client %d after %d ms",
        c.hostname,
        c.clientnum,
        servmillis - c.connectmillis);
    //--totalclients;
    c.peer->data = NULL;
    // enet_peer_disconnect(c->peer, reason);
    // (no need to notify other clients)
}

inline void serverslice(uint timeout = 5)
{
    servmillis = (int)enet_time_get();

    // server LAN socket
    serverinfo::process();

    // camera packets
    static int lastCameraPacket = 0;
    if (servmillis > lastCameraPacket + 500)
    {
        // TODO: send random data
        lastCameraPacket = servmillis;
    }

    static unsigned int lastThrottleEpoch = 0;
    if(serverhost->bandwidthThrottleEpoch != lastThrottleEpoch)
    {
        //if(lastThrottleEpoch) linequalitystats(serverhost->bandwidthThrottleEpoch - lastThrottleEpoch);
        lastThrottleEpoch = serverhost->bandwidthThrottleEpoch;
    }

    ENetEvent event;
    bool serviced = false;
    while(!serviced)
    {
        if(enet_host_check_events(serverhost, &event) <= 0)
        {
            if(enet_host_service(serverhost, &event, timeout) <= 0)
                break;

            serviced = true;
        }

        client *c = event.peer ? (client *)event.peer->data : NULL;
        switch(event.type)
        {
            case ENET_EVENT_TYPE_CONNECT:
            {
                c = &addclient();
                c->type = ST_IP;
                c->peer = event.peer;
                c->peer->data = &c;
                c->connectmillis = servmillis;
                if (enet_address_get_host_ip(&c->peer->address, c->hostname, sizeof(c->hostname)) != 0)
                    copystring(c->hostname, "unknown");
                logline(LOGL_INFO, "[%s] client connected", c->hostname);
                break;
            }

            case ENET_EVENT_TYPE_RECEIVE:
            {
                if (c != NULL)
                    process(event.packet, *c, static_cast<chan_t>(event.channelID));

                if (event.packet->referenceCount == 0)
                    enet_packet_destroy(event.packet);
                break;
            }

            case ENET_EVENT_TYPE_DISCONNECT:
            {
                disconnect_client(*c);
                break;
            }

            default:
                break;
        }
    }
}

int main(int argc, char **argv)
{
    // ENet init
    if (enet_initialize () != 0)
    {
        fputs("An error occurred while initializing ENet.\n", stderr);
        return EXIT_FAILURE;
    }

    // check args
    loopi(argc)
    {
        if (!scl.checkarg(argv[i]) && argv[i][0] == '-')
            fputs("WARNING: unknown commandline option\n", stderr);
    }

    // logging
    if(!initlogging(scl.logtimestamp))
        fputs("WARNING: logging not started!\n", stderr);

    // server info sockets
    serverinfo::init(scl.ip, scl.serverport+1);

    // ENet host
    ENetAddress address = { ENET_HOST_ANY, (enet_uint16)scl.serverport };
    if(scl.ip[0] && enet_address_set_host(&address, scl.ip)<0)
        logline(LOGL_WARNING, "server ip not resolved!");

    serverhost = enet_host_create(&address, scl.maxclients+1, CHAN_NUM, 0, scl.uprate);
    if (!serverhost)
        fatal("could not create server host");

    atexit(enet_deinitialize);
    enet_time_set(0);

    // started
    logline(LOGL_INFO, "server started, waiting for clients...");
    logline(LOGL_INFO, "Ctrl-C to exit");

    for(;;) // infinite server loop
        serverslice();

    return EXIT_SUCCESS;
}
