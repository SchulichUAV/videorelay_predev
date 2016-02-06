// Main application frame
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
    SetStatusText( "Welcome to wxWidgets!" );

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
}

ClientFrame::~ClientFrame()
{
    m_mgr.UnInit();
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