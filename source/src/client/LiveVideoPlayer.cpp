// Live Video Player
#include "../shared/pch.h"
#include "LiveVideoPlayer.h"

BEGIN_EVENT_TABLE(LiveVideoPlayer, wxPanel)
    EVT_PAINT(LiveVideoPlayer::OnPaint)
    EVT_SIZE(LiveVideoPlayer::OnSize)
END_EVENT_TABLE()

LiveVideoPlayer::LiveVideoPlayer(wxWindow *parent, wxWindowID id)
    : wxPanel(parent, id), m_Image(100, 100), m_bNewImage(true), m_bDrawLock(false)
{
}

/*
void LiveVideoPlayer::setSourceSize(uint width, uint height) {
    m_width = width;
    m_height = height;
}
*/

void LiveVideoPlayer::UpdateImage(uchar *buf) {
    size_t len = 100 * 100 * 3 * sizeof(uchar);
    uchar *newBuf = (uchar *)malloc(len);
    memcpy(newBuf, buf, len);

    m_Image.SetData(newBuf);
    m_bNewImage = true;
    Refresh(false);
    Update();
}

void LiveVideoPlayer::OnPaint(wxPaintEvent &event)
{
    wxPaintDC dc(this);
    render(dc);
}

void LiveVideoPlayer::OnSize(wxSizeEvent &event)
{
    m_bNewImage = true;
    Refresh(false);
    Update();
}

void LiveVideoPlayer::render(wxDC & dc)
{
    if (!dc.IsOk() || m_bDrawLock || !m_bNewImage) return;

    m_bDrawLock = true;

    wxSize &size = GetSize();
    const int w = size.GetWidth();
    const int h = size.GetHeight();
    const int x = size.GetX();
    const int y = size.GetY();

    dc.DrawBitmap(wxBitmap(m_Image.Scale(w, h)), 0, 0);

    m_bNewImage = false;

    m_bDrawLock = false;
}
