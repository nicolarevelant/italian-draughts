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
		m_size = size;
		return true;
	}
	return false;
}

wxSize ChessboardSquare::DoGetBestClientSize() const {
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

void ChessboardSquare::SetBorder(const wxPen &brd) {
	m_border = brd;
}

void ChessboardSquare::SetBitmap(const wxBitmap &bmp) {
	m_bitmap = bmp;
}


