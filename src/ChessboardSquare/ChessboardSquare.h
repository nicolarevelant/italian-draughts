// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2023  Nicola Revelant

#ifndef CHESSBOARD_SQUARE_H
#define CHESSBOARD_SQUARE_H

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
	 * Set the background bitmap (drawn before the other one)
	 * @param bitmap The new background bitmap
	 */
	void SetBackgroundBitmap(const wxBitmap &bitmap = wxNullBitmap);

	/**
	 * Set the foreground bitmap (drawn after the other one)
	 * @param bitmap The new foreground bitmap
	 */
	void SetForegroundBitmap(const wxBitmap &bitmap = wxNullBitmap);

protected:
	wxSize DoGetBestClientSize() const override;

private:
	ChessboardSquare(const ChessboardSquare &); // prevents copy-constructor

	int mSize{};
	wxBitmap mBackBitmap, mForeBitmap;

	/**
	 * Paint event
	 */
	void OnPaint(wxPaintEvent &);
};


#endif // CHESSBOARD_SQUARE_H
