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
    int port;
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

    std::vector<serverinfo *> servers;
    ENetSocket pingsock = ENET_SOCKET_NULL;

private:
    void OnTimer(wxTimerEvent& event);

    wxDECLARE_EVENT_TABLE();
};

#endif