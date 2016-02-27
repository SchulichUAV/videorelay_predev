#ifndef WX_CLIENT_FRAME_H
#define WX_CLIENT_FRAME_H

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <wx/aui/framemanager.h>

class ClientFrame: public wxFrame
{
public:
    ClientFrame();
    void SetAndLogMsg(const wxString &str);
    void AbortConnect();
    void Connect(const ENetAddress &address);

protected:
    ClientFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
    ~ClientFrame();
    void OnHello(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    wxDECLARE_EVENT_TABLE();

    wxAuiManager m_mgr;
    class ServerList *m_pServerList;
    class TextConsole *m_pTextConsole, *m_pTextIncoming;
    ENetHost *clienthost;
    ENetPeer *connpeer;
};

enum
{
    ID_Hello = 1
};

#endif
