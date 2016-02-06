#ifndef WX_CLIENT_FRAME_H
#define WX_CLIENT_FRAME_H

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

class ClientFrame: public wxFrame
{
public:
    ClientFrame();
private:
    ClientFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
    void OnHello(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    wxDECLARE_EVENT_TABLE();
};

enum
{
    ID_Hello = 1
};

#endif
