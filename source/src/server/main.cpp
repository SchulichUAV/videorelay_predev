// Main entry point of server application
#include "../shared/pch.h"
#include "server.h"

servercommandline scl;
ENetHost *serverhost;
uint servmillis = 0;
uint totalclients = 0;
client clients[MAXCLIENTS];

bool valid_client(int cn)
{
    return cn >= 0 && cn < MAXCLIENTS && clients[cn].type != ST_EMPTY;
}

void sendpacket(client *cl, chan_t chan, ENetPacket *packet, int exclude = -1)
{
    if (!cl)
    {
        /*
        loopi(MAXCLIENTS)
            if (i != exclude && clients[i].type != ST_EMPTY && clients[i].type != ST_REMOTE)
                sendpacket(&clients[i], chan, packet);
        */
        enet_host_broadcast(serverhost, chan, packet);
        return;
    }

    switch (cl->type)
    {
        case ST_REMOTE:
            enet_peer_send(cl->peer, chan, packet);
            break;
    }
}

client &addclient(client_type_t type = ST_REMOTE)
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
    logline(LOGL_INFO, "[%s] disconnected client (%d) after %u ms",
        c.hostname,
        c.clientnum,
        servmillis - c.connectmillis);
    --totalclients;
    c.peer->data = (void *)-1;
    if(reason >= 0) enet_peer_disconnect(c.peer, reason);
    // (no need to notify other clients)
    c.clear();
}

inline void serverslice(uint timeout = 5)
{
    servmillis = enet_time_get();

    // server LAN/info sockets
    serverinfo::process();

    // camera packets
    static uint lastCameraPacket = 0;
    if (servmillis > lastCameraPacket + 500)
    {
        lastCameraPacket = servmillis;
        const bool updated = true;
        // TODO add code to read camera
        if (totalclients && updated)
        {
            ENetPacket *packet;
            defformatstring(consoleDebugText)("server time = %d", servmillis);
            packet = enet_packet_create(consoleDebugText, strlen(consoleDebugText) * sizeof(char), 0);
            sendpacket(NULL, CHAN_TEXT, packet);

#define FRAMEBYTES 100 * 100 * 3
            static uchar videobuf[FRAMEBYTES];
            uchar *p = videobuf, *guard = &videobuf[FRAMEBYTES];
            // randomly update pixel bytes
            do
            {
                uint r = randomMT();
                *p = (uchar)r;
                p += ((r >> 8) & 0xF) + 1;
            } while (p < guard);

            // TODO manage memory and use ENET_PACKET_FLAG_NO_ALLOCATE
            packet = enet_packet_create(videobuf, FRAMEBYTES * sizeof(uchar), 0);
            sendpacket(NULL, CHAN_VIDEO, packet);
            enet_host_flush(serverhost);
        }
    }

    static uint lastThrottleEpoch = 0;
    if(serverhost->bandwidthThrottleEpoch != lastThrottleEpoch)
    {
        //if(lastThrottleEpoch) linequalitystats(serverhost->bandwidthThrottleEpoch - lastThrottleEpoch);
        lastThrottleEpoch = serverhost->bandwidthThrottleEpoch;
    }

    static uint nextstatus = 0;
    if (servmillis > nextstatus) // display bandwidth stats
    {
        nextstatus = servmillis + 60 * 1000;
        // no configs to reread
        if (totalclients)
        {
            logline(LOGL_INFO, "Status at %s: %u clients, %.1f out, %.1f in (KB/sec)",
                timestring(true, "%Y-%m-%d %H:%M:%S"),
                totalclients,
                serverhost->totalSentData / 60.0f / 1024,
                serverhost->totalReceivedData / 60.0f / 1024);
            // linequalitystats()
        }
        serverhost->totalSentData = serverhost->totalReceivedData = 0;
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

        switch(event.type)
        {
            case ENET_EVENT_TYPE_CONNECT:
            {
                client *c = &addclient();
                c->type = ST_REMOTE;
                c->peer = event.peer;
                c->peer->data = (void *)(size_t)c->clientnum;
                c->connectmillis = servmillis;
                if (enet_address_get_host_ip(&c->peer->address, c->hostname, sizeof(c->hostname)) != 0)
                    copystring(c->hostname, "unknown");
                concatformatstring(c->hostname, ":%u", event.peer->address.port);
                logline(LOGL_INFO, "[%s] client (%d) connected", c->hostname, c->clientnum);
                ++totalclients;
                break;
            }

            case ENET_EVENT_TYPE_RECEIVE:
            {
                int cn = (int)(size_t)event.peer->data;
                if (valid_client(cn))
                    process(event.packet, clients[cn], static_cast<chan_t>(event.channelID));
                if (event.packet->referenceCount == 0)
                    enet_packet_destroy(event.packet);
                break;
            }

            case ENET_EVENT_TYPE_DISCONNECT:
            {
                int cn = (int)(size_t)event.peer->data;
                if (!valid_client(cn))
                    break;
                disconnect_client(clients[cn]);
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
