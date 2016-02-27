// Main application frame
#include "../../shared/pch.h"
#include "ClientFrame.h"
#include "ServerList.h"
#include "TextConsole.h"

wxBEGIN_EVENT_TABLE(ClientFrame, wxFrame)
    EVT_MENU(ID_Hello,   ClientFrame::OnHello)
    EVT_MENU(wxID_EXIT,  ClientFrame::OnExit)
    EVT_MENU(wxID_ABOUT, ClientFrame::OnAbout)
wxEND_EVENT_TABLE()

ClientFrame::ClientFrame()
        : ClientFrame( "Video Feed App", wxPoint(50, 50), wxSize(960, 540) ) {}

#define CLIENTFRAME_STYLE (wxDEFAULT_FRAME_STYLE) // & ~(wxMAXIMIZE_BOX | wxRESIZE_BORDER))

ClientFrame::ClientFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
        : wxFrame(NULL, wxID_ANY, title, pos, size, CLIENTFRAME_STYLE)
{
    wxMenu *menuFile = new wxMenu;
    menuFile->Append(ID_Hello, "&Hello...\tCtrl-H",
                     "Help string shown in status bar for this menu item");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);
    wxMenu *menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);
    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append( menuFile, "&File" );
    menuBar->Append( menuHelp, "&Help" );
    SetMenuBar( menuBar );
    CreateStatusBar();
    SetStatusText( "Welcome to an app powered by wxWidgets!" );

    // Create panels
    m_mgr.SetManagedWindow(this);
    
    wxTextCtrl* video_temp = new wxTextCtrl(this, -1, "<video goes here>");
    m_pServerList = new ServerList(this, -1);
    m_pTextConsole = new TextConsole(this, -1);
    m_pTextIncoming = new TextConsole(this, -1);

    m_mgr.AddPane(video_temp, wxCENTER, "Video Feed");

    wxAuiPaneInfo paneInfo;
    paneInfo.Direction(wxRIGHT);
    paneInfo.Caption("Server List");
    paneInfo.CloseButton(false);
    m_mgr.AddPane(m_pServerList, wxRIGHT, "Server List");
    //m_mgr.AddPane(m_pServerList, paneInfo);
    m_mgr.AddPane(m_pTextConsole, wxRIGHT, "Console");
    m_mgr.AddPane(m_pTextIncoming, wxRIGHT, "Incoming Data");

    m_mgr.Update();

    clienthost = enet_host_create(NULL, // client host
        1, // 1 outgoing connection
        CHAN_NUM,
        0, // unlimited downstream bandwidth
        14400); // 14k upstream
    if (!clienthost)
    {
        fputs("Client host could not be created.\n", stderr);
        Close(true);
    }
    connpeer = NULL;
}

void ClientFrame::SetAndLogMsg(const wxString & str)
{
    SetStatusText(str);
    m_pTextConsole->AddLine(str);
}

void ClientFrame::AbortConnect()
{
    if (!connpeer)
        return;

    if (connpeer->state != ENET_PEER_STATE_DISCONNECTED)
        enet_peer_reset(connpeer);

    connpeer = NULL;
}

void ClientFrame::Connect(const ENetAddress &address)
{
    if (connpeer)
    {
        // This message would be overwritten in the status bar
        m_pTextConsole->AddLine("Aborting connection attempt");
        AbortConnect();
    }

    SetAndLogMsg(
        wxString::Format("Connecting to %s:%d",
            iptoa(ENET_NET_TO_HOST_32(address.host)),
            address.port));

    connpeer = enet_host_connect(clienthost, &address, CHAN_NUM, 0);
    if (connpeer == NULL)
    {
        SetAndLogMsg("Connect failed: no available ENet peers");
        return;
    }

    enet_host_flush(clienthost);

    // This is for testing only!
    ENetEvent event;
    /* Wait up to 1000 milliseconds for an event. */
    while (enet_host_service(clienthost, &event, 1000) > 0)
    {
        switch (event.type)
        {
        case ENET_EVENT_TYPE_CONNECT:
            printf("Connected from %x:%u.\n",
                event.peer->address.host,
                event.peer->address.port);
            /* Store any relevant client information here. */
            event.peer->data = "Client information";
            break;
        case ENET_EVENT_TYPE_RECEIVE:
            printf("A packet of length %u containing %s was received on channel %u.\n",
                (uint)event.packet->dataLength,
                (char *)event.packet->data,
                (uint)event.channelID);
            /* Clean up the packet now that we're done using it. */
            enet_packet_destroy(event.packet);

            break;

        case ENET_EVENT_TYPE_DISCONNECT:
            printf("DISCONNECTED");
            /* Reset the peer's client information. */
            event.peer->data = NULL;
        }
    }
}

ClientFrame::~ClientFrame()
{
    m_mgr.UnInit();
    enet_host_destroy(clienthost);
}

void ClientFrame::OnExit(wxCommandEvent& event)
{
    Close( true );
}
void ClientFrame::OnAbout(wxCommandEvent& event)
{
    wxMessageBox( "This is a wxWidgets' Hello world sample",
                  "About Hello World", wxOK | wxICON_INFORMATION );
}
void ClientFrame::OnHello(wxCommandEvent& event)
{
    wxLogMessage("Hello world from wxWidgets!");
}