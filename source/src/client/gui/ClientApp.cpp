// Implementation of client application
#include "../../shared/pch.h"
#include "ClientApp.h"
#include "ClientFrame.h"

wxIMPLEMENT_APP_NO_MAIN(ClientApp);

bool ClientApp::OnInit()
{
    m_pFrame = new ClientFrame();
    m_pFrame->Show( true );
    return true;
}

int ClientApp::OnExit()
{
    // delete m_pFrame?

    return wxApp::OnExit();
}
