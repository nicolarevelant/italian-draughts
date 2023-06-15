// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2023  Nicola Revelant

#include "ChessboardGrid.h"
#include "ProjectVariables.h"

ChessboardGrid::ChessboardGrid() = default;

ChessboardGrid::~ChessboardGrid() = default;

ChessboardGrid::ChessboardGrid(const wxColour &darkColor, const wxColour &lightColor, int squareSize,
                               wxWindow *parent, wxWindowID winId,
                               const wxPoint &pos) : ChessboardGrid() {
	if (!Create(darkColor, lightColor, squareSize, parent, winId, pos))
		throw std::exception();
}

bool ChessboardGrid::Create(const wxColour &darkColor, const wxColour &lightColor, int squareSize,
                            wxWindow *parent, wxWindowID winId,
                            const wxPoint &pos) {
	if (!wxPanel::Create(parent, winId, pos, wxSize{squareSize * 8, squareSize * 8}))
		return false;

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
	m_squareSize = squareSize;

	Layout();
	return true;
}

void ChessboardGrid::updateIcons(const wxBitmap &firstPawn, const wxBitmap &firstDame,
								 const wxBitmap &secondPawn, const wxBitmap &secondDame) {
	mFirstPawn = firstPawn;
	mFirstDame = firstDame;
	mSecondPawn = secondPawn;
	mSecondDame = secondDame;
}

void ChessboardGrid::OnItemMouseClicked(wxMouseEvent &evt) {
	wxPostEvent(this, evt);
}

void ChessboardGrid::updateDisposition(const GameUtils::Disposition &newDisposition, bool pcIsFirstPlayer) {
	for (int i = 0; i < 64; i++) {
		chessboard[i]->SetBorder();
		switch (newDisposition[i]) {
			case GameUtils::EMPTY:
				chessboard[i]->SetBitmap(wxNullBitmap);
				break;
			case GameUtils::PC_PAWN:
				chessboard[i]->SetBitmap(pcIsFirstPlayer ? mFirstPawn : mSecondPawn);
				break;
			case GameUtils::PC_DAME:
				chessboard[i]->SetBitmap(pcIsFirstPlayer ? mFirstDame : mSecondDame);
				break;
			case GameUtils::PL_PAWN:
				chessboard[i]->SetBitmap(pcIsFirstPlayer ? mSecondPawn : mFirstPawn);
				break;
			case GameUtils::PL_DAME:
				chessboard[i]->SetBitmap(pcIsFirstPlayer ? mSecondDame : mFirstDame);
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
