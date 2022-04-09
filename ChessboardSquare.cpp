#include "ChessboardSquare.h"

void ChessboardSquare::Init() {
    Bind(wxEVT_PAINT, &ChessboardSquare::OnPaint, this);
}

bool ChessboardSquare::Create(wxWindow *parent, wxWindowID windowId, const wxPen &brd, const wxBitmap &bmp) {
    if (wxWindow::Create(parent, windowId)) {
        m_border = brd;
        m_bitmap = bmp;
        Layout();
        return true;
    }
    return false;
}

wxSize ChessboardSquare::DoGetBestSize() const {
    return {m_size, m_size};
}

void ChessboardSquare::OnPaint(wxPaintEvent &) {
    wxPaintDC dc(this);

    dc.SetBrush(m_transparentBrush);
    dc.SetPen(m_border);
    dc.DrawRectangle(0, 0, m_size, m_size);

    if (dc.CanDrawBitmap() && m_bitmap.IsOk())
        dc.DrawBitmap(m_bitmap, 0, 0);
}

wxPen &ChessboardSquare::GetBorder() {
    return m_border;
}

void ChessboardSquare::SetBorder(const wxPen &brd) {
    m_border = brd;
}

void ChessboardSquare::SetBitmap(const wxBitmap &bmp) {
    m_bitmap = bmp;
}


