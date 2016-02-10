#ifndef WX_CLIENT_APP_H
#define WX_CLIENT_APP_H

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

class ClientApp: public wxApp
{
public:
    virtual bool OnInit();
    virtual int OnExit();

protected:
    class ClientFrame *m_pFrame;
};

#endif
