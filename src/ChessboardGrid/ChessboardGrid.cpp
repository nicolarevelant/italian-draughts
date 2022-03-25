// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2023  Nicola Revelant

#include "ChessboardGrid.h"
#include "ProjectVariables.h"

ChessboardGrid::ChessboardGrid() = default;

ChessboardGrid::~ChessboardGrid() = default;

ChessboardGrid::ChessboardGrid(const wxColour &darkColor, const wxColour &lightColor,
                               wxWindow *parent, wxWindowID winId,
                               const wxPoint &pos, int squareSize) : ChessboardGrid() {
	if (!Create(darkColor, lightColor, parent, winId, pos, squareSize))
		throw std::exception();
}

bool ChessboardGrid::Create(const wxColour &darkColor, const wxColour &lightColor,
                            wxWindow *parent, wxWindowID winId,
                            const wxPoint &pos, int sqSize) {
	if (!wxPanel::Create(parent, winId, pos, wxSize{sqSize * 8, sqSize * 8}))
		return false;

	if (!pcPawn.LoadFile(SYSTEM_CFG_PATH"images/pcPawn.png") ||
	    !pcDame.LoadFile(SYSTEM_CFG_PATH"images/pcDame.png") ||
	    !plPawn.LoadFile(SYSTEM_CFG_PATH"images/plPawn.png") ||
	    !plDame.LoadFile(SYSTEM_CFG_PATH"images/plDame.png"))
		return false;

	squareSize = sqSize;

	auto *gridSizer = new wxGridSizer(8, 8, 0, 0);
	SetSizer(gridSizer);

	SetBackgroundColour(lightColor);

	ChessboardSquare *square;
	for (int row = 0, i = 0, col; row < 8; row++) {
		for (col = 0; col < 8; col++, i++) {
			square = new ChessboardSquare(squareSize, this, i);
			if (row % 2 == col % 2) {
				square->SetBackgroundColour(darkColor);
			} else {
				square->SetBackgroundColour(lightColor);
			}
			square->Bind(wxEVT_LEFT_UP, &ChessboardGrid::OnItemMouseClicked, this);
			gridSizer->Add(square);
			chessboard[i] = square;
		}
	}

	Layout();
	return true;
}

void ChessboardGrid::OnItemMouseClicked(wxMouseEvent &evt) {
	wxPostEvent(this, evt);
}

void ChessboardGrid::updateDisposition(const GameUtils::Disposition &newDisposition) {
	for (int i = 0; i < 64; i++) {
		chessboard[i]->SetBorder();
		switch (newDisposition[i]) {
			case GameUtils::EMPTY:
				chessboard[i]->SetBitmap(wxNullBitmap);
				break;
			case GameUtils::PC_PAWN:
				chessboard[i]->SetBitmap(pcPawn);
				break;
			case GameUtils::PC_DAME:
				chessboard[i]->SetBitmap(pcDame);
				break;
			case GameUtils::PL_PAWN:
				chessboard[i]->SetBitmap(plPawn);
				break;
			case GameUtils::PL_DAME:
				chessboard[i]->SetBitmap(plDame);
				break;
		}
	}
	wxWindow::Refresh();
}

void ChessboardGrid::SetSquareBorder(int index, const wxPen &border) {
	chessboard[index]->SetBorder(border);
	wxWindow::Refresh();
}

void ChessboardGrid::ClearSquareBorders() {
	for (int i = 0; i < 64; i++) {
		chessboard[i]->SetBorder();
	}
	wxWindow::Refresh();
}
