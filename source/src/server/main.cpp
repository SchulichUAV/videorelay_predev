// Main entry point of server application
#include "../shared/pch.h"

inline void serverslice(uint timeout = 5)
{
    // server LAN socket

    // send camera packets

    static unsigned int lastThrottleEpoch = 0;
    if(serverhost->bandwidthThrottleEpoch != lastThrottleEpoch)
    {
        if(lastThrottleEpoch) linequalitystats(serverhost->bandwidthThrottleEpoch - lastThrottleEpoch);
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
        switch(event.type)
        {
            case ENET_EVENT_TYPE_CONNECT:
            {
                cout << "client connected\n";
                break;
            }

            case ENET_EVENT_TYPE_RECEIVE:
            {
                int cn = (int)(size_t)event.peer->data;
                cout << "packet received\n";
                if(event.packet->referenceCount==0) enet_packet_destroy(event.packet);
                break;
            }

            case ENET_EVENT_TYPE_DISCONNECT:
            {
                cout << "client disconnected\n";
                break;
            }

            default:
                break;
        }
    }
}

int main(int argn, const char **argc)
{
    if (enet_initialize () != 0)
    {
        cerr << "An error occurred while initializing ENet." << endl;
        return EXIT_FAILURE;
    }

    int serverport = 1337;
    const char *ip = "";
    int maxclients = 128;
    int uprate = 0;

    loopi(argc)
    {
        // read arguments
    }

    ENetAddress address = { ENET_HOST_ANY, (enet_uint16)serverport };
    if(scl.ip[0] && enet_address_set_host(&address, scl.ip)<0)
        cout << "WARNING: server ip not resolved!" << endl;

    serverhost = enet_host_create(&address, maxclients+1, 2, 0, uprate);
    if(!serverhost) fatal("could not create server host");

    atexit(enet_deinitialize);
    enet_time_set(0);

    // Enter infinite server loop
    for(;;)
        serverslice();

    return EXIT_SUCCESS;
}
