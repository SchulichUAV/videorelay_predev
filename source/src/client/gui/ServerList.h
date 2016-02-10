#ifndef WX_SERVER_LIST_H
#define WX_SERVER_LIST_H

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/listctrl.h>

class ServerList : public wxPanel
{
public:
    ServerList(wxWindow *parent, wxWindowID id = -1);

protected:
    wxBoxSizer *m_Sizer;
    wxListCtrl *m_List;
};

#endif