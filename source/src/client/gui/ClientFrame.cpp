// Main application frame
#include "ClientFrame.h"

wxBEGIN_EVENT_TABLE(ClientFrame, wxFrame)
    EVT_MENU(ID_Hello,   ClientFrame::OnHello)
    EVT_MENU(wxID_EXIT,  ClientFrame::OnExit)
    EVT_MENU(wxID_ABOUT, ClientFrame::OnAbout)
wxEND_EVENT_TABLE()

ClientFrame::ClientFrame()
        : ClientFrame( "Video Feed App", wxPoint(50, 50), wxSize(960, 540) ) {}

ClientFrame::ClientFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
        : wxFrame(NULL, wxID_ANY, title, pos, size)
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