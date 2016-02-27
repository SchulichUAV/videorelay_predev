#ifndef WX_TEXT_CONSOLE_H
#define WX_TEXT_CONSOLE_H

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

class TextConsole : public wxTextCtrl
{
public:
    TextConsole(wxWindow *parent, wxWindowID id = -1);
    void AddLine(const wxString &line);
};

#endif
