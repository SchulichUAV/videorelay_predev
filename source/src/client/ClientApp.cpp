// Implementation of client application
#include "ClientApp.h"
#include "gui/ClientFrame.h"

wxIMPLEMENT_APP_NO_MAIN(ClientApp);

bool ClientApp::OnInit()
{
    ClientFrame *frame = new ClientFrame();
    frame->Show( true );
    return true;
}
