// Implementation of server list control
#include "ServerList.h"

ServerList::ServerList(wxWindow *parent, wxWindowID id) : wxPanel(parent, id)
{
    m_pList = new wxListCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT);
    m_pList->AppendColumn("Server");
    m_pList->AppendColumn("Ping");

    m_pSizer = new wxBoxSizer(wxHORIZONTAL);
    m_pSizer->Add(m_pList, 1, wxEXPAND | wxALL);
    SetSizer(m_pSizer);
}
