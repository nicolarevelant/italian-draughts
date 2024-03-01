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

#include "ChessboardSquare.h"

ChessboardSquare::ChessboardSquare() {
	Bind(wxEVT_PAINT, &ChessboardSquare::OnPaint, this);
}

ChessboardSquare::ChessboardSquare(int size, wxWindow *parent, wxWindowID windowId) : ChessboardSquare() {
	if (!Create(size, parent, windowId)) {
#ifdef DEBUG
		std::cerr << "Cannot create ChessboardSquare" << std::endl;
#endif
		throw std::exception();
	}
}

bool ChessboardSquare::Create(int size, wxWindow *parent, wxWindowID windowId) {
	if (wxWindow::Create(parent, windowId)) {
		mSize = size;
		return true;
	}
	return false;
}

wxSize ChessboardSquare::DoGetBestClientSize() const {
	return {mSize, mSize};
}

void ChessboardSquare::OnPaint(wxPaintEvent &) {
	wxPaintDC dc(this);

	if (dc.CanDrawBitmap() && mBackBitmap.IsOk()) {
		dc.DrawBitmap(mBackBitmap, 0, 0);
	}

	if (dc.CanDrawBitmap() && mForeBitmap.IsOk()) {
		dc.DrawBitmap(mForeBitmap, 0, 0);
	}
}

void ChessboardSquare::SetBackgroundBitmap(const wxBitmap &bitmap) {
	mBackBitmap = bitmap;
}

void ChessboardSquare::SetForegroundBitmap(const wxBitmap &bitmap) {
	mForeBitmap = bitmap;
}
