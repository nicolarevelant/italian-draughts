#ifndef ITALIAN_DRAUGHTS_CHESSBOARD_SQUARE_H
#define ITALIAN_DRAUGHTS_CHESSBOARD_SQUARE_H

#include "Resources.h"

#define DEF_SQUARE_SIZE 74

class ChessboardSquare : public wxWindow {
public:
    ChessboardSquare() : m_size(DEF_SQUARE_SIZE) { Init(); }
    ~ChessboardSquare() override = default;

    explicit ChessboardSquare(wxWindow *parent,
                              wxWindowID windowId = wxID_ANY,
                              int size = DEF_SQUARE_SIZE,
                              const wxPen &border = wxNullPen,
                              const wxBitmap &bitmap = wxNullBitmap)
                              : m_size(size) {
        Init();
        Create(parent, windowId, border, bitmap);
    }

    bool Create(wxWindow *parent,
                wxWindowID windowId = wxID_ANY,
                const wxPen &border = wxNullPen,
                const wxBitmap &bitmap = wxNullBitmap);

    void SetBorder(const wxPen &border);

    void SetBitmap(const wxBitmap &bitmap);

private:
    const int m_size;
    wxBitmap m_bitmap = wxNullBitmap;
    const wxBrush m_transparentBrush = wxBrush(wxTransparentColour, wxBRUSHSTYLE_TRANSPARENT);
    wxPen m_border = wxNullPen;

    void Init();

    // needed for having size*size as forced size
    [[nodiscard]] wxSize DoGetBestSize() const override;

    void OnPaint(wxPaintEvent &);
};


#endif //ITALIAN_DRAUGHTS_CHESSBOARD_SQUARE_H
