#ifndef WX_SERVER_LIST_H
#define WX_SERVER_LIST_H

#include "../../shared/pch.h"
#include <wx/listctrl.h>

enum
{
    ID_SERVERLIST_TIMER = wxID_HIGHEST,
};

struct serverinfo
{
    string name;
    ENetAddress address;
    int port, ping;

    serverinfo(const ENetAddress &addr) : address(addr)
    {
        enet_address_get_host_ip(&addr, name, sizeof(name));

        // recover original port
        port = addr.port - 1;

        ping = -1;
    }
};

class ServerList : public wxPanel
{
public:
    ServerList(wxWindow *parent, wxWindowID id = -1);
    void refreshservers(bool init = false);

protected:
    void checkresolver();
    void checkpings();
    void pingservers();

    wxBoxSizer *m_pSizer;
    wxListCtrl *m_pList;
    wxTimer m_Timer;

    std::vector<serverinfo> servers;
    ENetSocket pingsock = ENET_SOCKET_NULL;

private:
    void OnTimer(wxTimerEvent& event);

    wxDECLARE_EVENT_TABLE();
};

#endif