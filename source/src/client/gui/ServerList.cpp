// Implementation of server list control
#include "../../shared/pch.h"
#include "ServerList.h"

BEGIN_EVENT_TABLE(ServerList, wxPanel)
    EVT_TIMER(ID_SERVERLIST_TIMER, ServerList::OnTimer)
END_EVENT_TABLE()

ServerList::ServerList(wxWindow *parent, wxWindowID id)
    : wxPanel(parent, id),
      m_Timer(this, ID_SERVERLIST_TIMER)
{
    m_pList = new wxListCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT);
    m_pList->AppendColumn("Server");
    m_pList->AppendColumn("Ping");

    m_pSizer = new wxBoxSizer(wxHORIZONTAL);
    m_pSizer->Add(m_pList, 1, wxEXPAND | wxALL);
    SetSizer(m_pSizer);

    //m_pTimer = new wxTimer(this, ID_SERVERLIST_TIMER);
    //m_pTimer->Start(200);
    m_Timer.Start(200);

    refreshservers(true);
}

void ServerList::pingservers()
{
    if(pingsock == ENET_SOCKET_NULL)
    {
        pingsock = enet_socket_create(ENET_SOCKET_TYPE_DATAGRAM);
        if(pingsock == ENET_SOCKET_NULL)
        {
            //lastinfo = totalmillis;
            return;
        }
        enet_socket_set_option(pingsock, ENET_SOCKOPT_NONBLOCK, 1);
        enet_socket_set_option(pingsock, ENET_SOCKOPT_BROADCAST, 1);
    }

    ENetBuffer buf;
    static time_t ping[1];
    ping[0] = time(NULL);

    bool searchlan = true;

    for (auto it = servers.begin(); it != servers.end(); ++it)
    {
        serverinfo &si = *it;
        if (si.address.host == ENET_HOST_ANY) continue;
        buf.data = ping;
        buf.dataLength = sizeof(ping);
        enet_socket_send(pingsock, &si.address, &buf, 1);
    }

    if (searchlan)
    {
        ENetAddress address;
        address.host = ENET_HOST_BROADCAST;
        address.port = SERVINFO_PORT_LAN;
        buf.data = ping;
        buf.dataLength = sizeof(ping);
        enet_socket_send(pingsock, &address, &buf, 1);
    }
}

void ServerList::checkresolver()
{
    // a resolver isn't needed yet
}

void ServerList::checkpings()
{
    if (pingsock == ENET_SOCKET_NULL) return;
    enet_uint32 events = ENET_SOCKET_WAIT_RECEIVE;
    ENetBuffer buf;
    ENetAddress addr;
    static time_t ping[1];
    buf.data = ping;
    buf.dataLength = sizeof(ping);
    while (enet_socket_wait(pingsock, &events, 0) >= 0 && events)
    {
        int len = enet_socket_receive(pingsock, &addr, &buf, 1);
        if (len <= 0) return;

        serverinfo *si = NULL;
        for (auto s = servers.begin(); s != servers.end(); ++s)
        {
            if (s->address.host == addr.host && s->address.port == addr.port)
            {
                si = &*s;
                break;
            }
        }

        if (!si)
        {
            servers.push_back(serverinfo(addr));
            si = &servers.back();
        }

        si->ping = time(NULL) - ping[0];
        printf("ping received: %d (after %d ms)", ping[0], (int)si->ping);
    }
}

void ServerList::refreshservers(bool init)
{
    checkresolver();
    checkpings();
    // TODO: also ping every 5 seconds
    if(init)
        pingservers();
}

void ServerList::OnTimer(wxTimerEvent& WXUNUSED(event))
{
    refreshservers();
}
