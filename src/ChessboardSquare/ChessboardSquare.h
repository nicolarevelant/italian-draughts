/*
    Copyright (C) 2023-2024  Nicola Revelant

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

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
