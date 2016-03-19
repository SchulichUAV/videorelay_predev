#ifndef WX_LIVE_VIDEO_PLAYER_H
#define WX_LIVE_VIDEO_PLAYER_H

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/mediactrl.h>

class LiveVideoPlayer : public wxPanel
{
public:
    LiveVideoPlayer(wxWindow *parent, wxWindowID id = -1);

    void UpdateImage(uchar *buf);

    //void setSourceSize(uint width, uint height);
    //wxImage &GetImage() { return m_image; }

private:
    void OnPaint(wxPaintEvent &event);
    void OnSize(wxSizeEvent &event);

    void render(wxDC &dc);

    //uint m_width = 1, m_height = 1;
    wxImage m_Image;
    bool m_bNewImage, m_bDrawLock;

    DECLARE_EVENT_TABLE()
};

#endif