// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2023  Nicola Revelant

#ifndef ITALIAN_DRAUGHTS_CHESSBOARD_GRID_H
#define ITALIAN_DRAUGHTS_CHESSBOARD_GRID_H

#include "ChessboardSquare/ChessboardSquare.h"
#include "GameUtils/GameUtils.h"
#include <wx/wx.h>

#define DEF_DARK_COLOR wxColour(32, 32, 32)
#define DEF_LIGHT_COLOR wxColour(140, 140, 140)

/**
 * The mission of this class is to provide a wxPanel composed by a 8x8 grid
 */
class ChessboardGrid : public wxPanel {
public:
	typedef std::function<const wxBitmap &(const std::string &, const wxBitmap &)> ImageProviderCB;
	typedef std::function<const wxColour &(const std::string &, const wxColour &)> ColorProviderCB;

	ChessboardGrid();

	~ChessboardGrid() override;

	/**
	 * Creates a new ChessboardGrid
	 * @param darkColor Color used in dark squares
	 * @param lightColor Color used in light squares
	 * @param squareSize Square size
	 * @param parent Parent of this wxPanel
	 * @param winId Window ID, or wxID_ANY
	 * @param pos Position relative to the parent
	 */
	ChessboardGrid(const ImageProviderCB &images, const ColorProviderCB &colors, wxWindow *parent,
	               wxWindowID winId = wxID_ANY, const wxPoint &pos = wxDefaultPosition);

	/**
	 * Creates a new ChessboardGrid using two-step construction
	 */
	bool Create(const ImageProviderCB &images, const ColorProviderCB &colors, wxWindow *parent,
	            wxWindowID winId = wxID_ANY, const wxPoint &pos = wxDefaultPosition);

	/**
	 * Updates the disposition of the pieces in the chessboard and clears every border
	 * @param newDisposition The new disposition to apply
	 * @param pcIsFirstPlayer True if PC is the first player
	 */
	void updateDisposition(const GameUtils::Disposition &newDisposition, bool pcIsFirstPlayer);

	/**
	 * Highlight the specified square as selected
	 * @param index Position of the square
	 */
	void SetSquareSelectedOverlay(int index);

	/**
	 * Highlight the specified square as possible move
	 * @param index Position of the square
	 */
	void SetSquarePossibleMoveOverlay(int index);

	/**
	 * Clears the foreground bitmap of all 64 squares
	 */
	void ClearSquareOverlay();

private:
	ChessboardGrid(const ChessboardGrid &); // prevents copy-constructor
	std::array<ChessboardSquare *, 64> chessboard{};

	wxBitmap mFirstPawn = wxNullBitmap, mFirstDame = wxNullBitmap, mSecondPawn = wxNullBitmap, mSecondDame = wxNullBitmap;
	wxBitmap mSelected = wxNullBitmap, mPossibleMove = wxNullBitmap;
	GameUtils::MoveList moves; // list of moves the player can do

	void OnItemMouseClicked(wxMouseEvent &evt);
};


#endif //ITALIAN_DRAUGHTS_CHESSBOARD_GRID_H
