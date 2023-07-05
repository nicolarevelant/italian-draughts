// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2023  Nicola Revelant

#include "ChessboardSquare.h"

ChessboardSquare::ChessboardSquare() {
	Bind(wxEVT_PAINT, &ChessboardSquare::OnPaint, this);
}

ChessboardSquare::ChessboardSquare(int size, wxWindow *parent, wxWindowID windowId) : ChessboardSquare() {
	if (!Create(size, parent, windowId))
		throw std::exception();
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
