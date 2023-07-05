// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2023  Nicola Revelant

#include "ChessboardGrid.h"
#include "ProjectVariables.h"

ChessboardGrid::ChessboardGrid() = default;

ChessboardGrid::~ChessboardGrid() = default;

ChessboardGrid::ChessboardGrid(const ImageProviderCB &images, const ColorProviderCB &colors,
							   wxWindow *parent, wxWindowID winId, const wxPoint &pos) : ChessboardGrid() {
	if (!Create(images, colors, parent, winId, pos))
		throw std::exception();
}

bool ChessboardGrid::Create(const ImageProviderCB &images, const ColorProviderCB &colors,
                            wxWindow *parent, wxWindowID winId, const wxPoint &pos) {
	if (!wxPanel::Create(parent, winId, pos))
		return false;

	mFirstDame = images("firstDame", wxNullBitmap);
	if (!mFirstDame.IsOk()) return false;
	auto size = mFirstDame.GetSize();
	mFirstPawn = images("firstPawn", wxNullBitmap);
	if (!mFirstPawn.IsOk() || mFirstPawn.GetSize() != size) return false;

	mSecondDame = images("secondDame", wxNullBitmap);
	if (!mSecondDame.IsOk() || mSecondDame.GetSize() != size) return false;

	mSecondPawn = images("secondPawn", wxNullBitmap);
	if (!mSecondPawn.IsOk() || mSecondPawn.GetSize() != size) return false;

	mSelected = images("selected", wxNullBitmap);
	if (!mSelected.IsOk() || mSelected.GetSize() != size) return false;

	mPossibleMove = images("possibleMove", wxNullBitmap);
	if (!mPossibleMove.IsOk() || mPossibleMove.GetSize() != size) return false;

	wxPanel::SetInitialSize(size * 8);

	auto *gridSizer = new wxGridSizer(8, 8, 0, 0);
	SetSizer(gridSizer);

	ChessboardSquare *square;
	for (int row = 0, i = 0, col; row < 8; row++) {
		for (col = 0; col < 8; col++, i++) {
			square = new ChessboardSquare(size.GetWidth(), this, i);
			if (row % 2 == col % 2) {
				square->SetBackgroundColour(colors("dark", DEF_DARK_COLOR));
			} else {
				square->SetBackgroundColour(colors("light", DEF_LIGHT_COLOR));
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

void ChessboardGrid::updateDisposition(const GameUtils::Disposition &newDisposition, bool pcIsFirstPlayer) {
	for (int i = 0; i < 64; i++) {
		chessboard[i]->SetForegroundBitmap();
		switch (newDisposition[i]) {
			case GameUtils::EMPTY:
				chessboard[i]->SetBackgroundBitmap();
				break;
			case GameUtils::PC_PAWN:
				chessboard[i]->SetBackgroundBitmap(pcIsFirstPlayer ? mFirstPawn : mSecondPawn);
				break;
			case GameUtils::PC_DAME:
				chessboard[i]->SetBackgroundBitmap(pcIsFirstPlayer ? mFirstDame : mSecondDame);
				break;
			case GameUtils::PL_PAWN:
				chessboard[i]->SetBackgroundBitmap(pcIsFirstPlayer ? mSecondPawn : mFirstPawn);
				break;
			case GameUtils::PL_DAME:
				chessboard[i]->SetBackgroundBitmap(pcIsFirstPlayer ? mSecondDame : mFirstDame);
				break;
		}
	}
	wxWindow::Refresh();
}

void ChessboardGrid::SetSquareSelectedOverlay(int index) {
	chessboard[index]->SetForegroundBitmap(mSelected);
	wxWindow::Refresh();
}

void ChessboardGrid::SetSquarePossibleMoveOverlay(int index) {
	chessboard[index]->SetForegroundBitmap(mPossibleMove);
	wxWindow::Refresh();
}

void ChessboardGrid::ClearSquareOverlay() {
	for (int i = 0; i < 64; i++) {
		chessboard[i]->SetForegroundBitmap();
	}
	wxWindow::Refresh();
}
