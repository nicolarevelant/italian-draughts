// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2023  Nicola Revelant

#ifndef ITALIAN_DRAUGHTS_CHESSBOARD_SQUARE_H
#define ITALIAN_DRAUGHTS_CHESSBOARD_SQUARE_H

#include <wx/wx.h>

class ChessboardSquare : public wxWindow {
public:
	ChessboardSquare();

	~ChessboardSquare() override = default;

	/**
	 * Creates a new chessboard square
	 * @param size Square size
	 * @param parent Parent
	 * @param windowId Window ID
	 */
	explicit ChessboardSquare(int size, wxWindow *parent, wxWindowID windowId = wxID_ANY);

	/**
	 * Creates the chessboard square using two-step construction
	 * @param size Square size
	 * @param parent Parent
	 * @param windowId Window ID
	 * @return
	 */
	bool Create(int size, wxWindow *parent,
	            wxWindowID windowId = wxID_ANY);

	/**
	 * Set a new border as wxPen
	 * @param border A new wxPen border
	 */
	void SetBorder(const wxPen &border = wxNullPen);

	/**
	 * Set a new image as wxBitmap
	 * @param bitmap A new wxBitmap image
	 */
	void SetBitmap(const wxBitmap &bitmap);

protected:
	wxSize DoGetBestClientSize() const override;

private:
	ChessboardSquare(const ChessboardSquare &); // prevents copy-constructor

	int m_size = 0;
	wxBitmap m_bitmap = wxNullBitmap;
	const wxBrush m_transparentBrush{wxTransparentColour, wxBRUSHSTYLE_TRANSPARENT};
	wxPen m_border = wxNullPen;

	/**
	 * Paint event
	 */
	void OnPaint(wxPaintEvent &);
};


#endif //ITALIAN_DRAUGHTS_CHESSBOARD_SQUARE_H
