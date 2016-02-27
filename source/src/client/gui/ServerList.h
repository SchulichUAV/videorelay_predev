#ifndef WX_SERVER_LIST_H
#define WX_SERVER_LIST_H

#include "../../shared/pch.h"
#include <wx/listctrl.h>

enum
{
    ID_SERVERLIST_TIMER = wxID_HIGHEST,
    ID_SERVERLIST_LISTVIEW,
};

struct serverinfo
{
    string name;
    ENetAddress address;
    int ping;
    long listID;
    bool lan;

    serverinfo(const ENetAddress &addr, long id, bool lan)
        : address(addr),
          ping(-1),
          listID(id),
          lan(lan)
    {
        if (enet_address_get_host(&addr, name, sizeof(name)) < 0)
            if (enet_address_get_host_ip(&addr, name, sizeof(name)) < 0)
                copystring(name, "unknown");
        // recover original port
        --address.port;
    }

    bool matchPongAddr(const ENetAddress &addr)
    {
        return address.host == addr.host &&
            address.port == addr.port - 1;
    }
};

class ServerList : public wxPanel
{
public:
    ServerList(ClientFrame *parent, wxWindowID id = -1);
    void refreshservers(bool init = false);

protected:
    void checkresolver();
    void checkpings();
    void pingservers();
    serverinfo &addServer(const ENetAddress &addr, bool lan);

    ClientFrame *m_pClientFrame;
    wxBoxSizer *m_pSizer;
    wxListCtrl *m_pList;
    wxTimer m_Timer;

    std::vector<serverinfo> servers;
    ENetSocket pingsock = ENET_SOCKET_NULL;

private:
    void OnTimer(wxTimerEvent &event);
    void OnListItemActivated(wxListEvent &event);

    wxDECLARE_EVENT_TABLE();
};

#endif