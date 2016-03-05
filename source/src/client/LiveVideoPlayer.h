#ifndef WX_LIVE_VIDEO_PLAYER_H
#define WX_LIVE_VIDEO_PLAYER_H

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/mediactrl.h>

class LiveVideoPlayer : public wxMediaCtrl
{
public:
    LiveVideoPlayer(wxWindow *parent, wxWindowID id = -1);
};

#endif