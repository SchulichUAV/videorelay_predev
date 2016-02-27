// Implementation of server list control
#include "../../shared/pch.h"
#include "ClientFrame.h"
#include "ServerList.h"

BEGIN_EVENT_TABLE(ServerList, wxPanel)
    EVT_TIMER(ID_SERVERLIST_TIMER, ServerList::OnTimer)
    EVT_LIST_ITEM_ACTIVATED(ID_SERVERLIST_LISTVIEW, ServerList::OnListItemActivated)
END_EVENT_TABLE()

ServerList::ServerList(ClientFrame *parent, wxWindowID id)
    : wxPanel(parent, id),
      m_pClientFrame(parent),
      m_Timer(this, ID_SERVERLIST_TIMER)
{
    m_pList = new wxListCtrl(this, ID_SERVERLIST_LISTVIEW, wxDefaultPosition, wxDefaultSize, wxLC_REPORT);
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
    static enet_uint32 ping[1];
    ping[0] = enet_time_get();

    bool searchlan = true;

    for (auto it = servers.begin(); it != servers.end(); ++it)
    {
        serverinfo &si = *it;
        if (si.lan || si.address.host == ENET_HOST_ANY) continue;
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
    static enet_uint32 ping[1];
    buf.data = ping;
    buf.dataLength = sizeof(ping);
    while (enet_socket_wait(pingsock, &events, 0) >= 0 && events)
    {
        int len = enet_socket_receive(pingsock, &addr, &buf, 1);
        if (len <= 0) return;

        serverinfo *si = NULL;
        for (auto s = servers.begin(); s != servers.end(); ++s)
        {
            if (s->matchPongAddr(addr))
            {
                si = &*s;
                break;
            }
        }

        if (si == NULL)
            si = &addServer(addr, true);

        si->ping = enet_time_get() - ping[0];
        m_pList->SetItem(si->listID, 1, wxString::Format("%d", si->ping));
    }
}

void ServerList::refreshservers(bool init)
{
    checkresolver();
    checkpings();
    static enet_uint32 lastping = 0;
    enet_uint32 curmillis = enet_time_get();
    if (init || curmillis > lastping + 5000) {
        lastping = curmillis;
        pingservers();
    }
}

serverinfo &ServerList::addServer(const ENetAddress &addr, bool lan)
{
    long id = m_pList->InsertItem(0, "");

    serverinfo si(addr, id, lan);
    m_pList->SetItem(id, 0, wxString::Format("%s:%d", si.name, si.address.port));
    m_pList->SetItem(id, 1, "?");

    servers.push_back(si);
    return servers.back();
}

void ServerList::OnTimer(wxTimerEvent& WXUNUSED(event))
{
    refreshservers();
}

void ServerList::OnListItemActivated(wxListEvent &event)
{
    long id = event.GetItem().GetId();

    serverinfo *si = NULL;
    for (auto s = servers.begin(); s != servers.end(); ++s)
    {
        if (s->listID == id)
        {
            si = &*s;
            break;
        }
    }

    if (si == NULL)
    {
        // This shouldn't happen, but if the serverinfo is not found,
        // just delete the entry.
        m_pClientFrame->SetAndLogMsg(
            wxString::Format("ERROR: serverinfo for ID %ld disappeared!", id));
        m_pList->DeleteItem(id);
        return;
    }

    m_pClientFrame->Connect(si->address);
}
