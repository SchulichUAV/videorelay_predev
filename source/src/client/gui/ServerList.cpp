// Implementation of server list control
#include "ServerList.h"

ServerList::ServerList(wxWindow *parent, wxWindowID id)
    : wxPanel(parent, id),
    m_Sizer(wxHORIZONTAL),
    m_List(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT)
{
    m_List.AppendColumn("Server");
    m_List.AppendColumn("Ping");

    m_Sizer.Add(&m_List, 1, wxEXPAND | wxALL);
    SetSizer(&m_Sizer);
}
