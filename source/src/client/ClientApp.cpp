// Implementation of client application
#include "../shared/pch.h"
#include "ClientApp.h"
#include "ClientFrame.h"

wxIMPLEMENT_APP(ClientApp);

bool ClientApp::OnInit()
{
    if (enet_initialize() != 0)
    {
        fputs("An error occurred while initializing ENet.\n", stderr);
        return false;
    }

    Connect(wxID_ANY, wxEVT_IDLE, wxIdleEventHandler(ClientApp::onIdle));

    m_pFrame = new ClientFrame();
    m_pFrame->Show( true );
    return true;
}

int ClientApp::OnExit()
{
    m_shuttingDown = true;
    Disconnect(wxEVT_IDLE, wxIdleEventHandler(ClientApp::onIdle));

    // TODO disconnect from server if connected

    enet_deinitialize();
    return wxApp::OnExit();
}

void ClientApp::onIdle(wxIdleEvent& evt)
{
    // TODO move to wxWidgets timer
    enet_uint32 now = enet_time_get();
    static enet_uint32 nextCheck = 0;

    if (m_shuttingDown || now < nextCheck)
        return;

    nextCheck = now + 500;

    m_pFrame->CheckNetwork();

    evt.RequestMore();
}
