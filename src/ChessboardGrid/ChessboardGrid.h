// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2023  Nicola Revelant

#ifndef ITALIAN_DRAUGHTS_CHESSBOARD_GRID_H
#define ITALIAN_DRAUGHTS_CHESSBOARD_GRID_H

#include "ChessboardSquare/ChessboardSquare.h"
#include "GameUtils/GameUtils.h"
#include <wx/wx.h>

/**
 * The mission of this class is to provide a wxPanel composed by a 8x8 grid
 */
class ChessboardGrid : public wxPanel {
public:
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
	ChessboardGrid(const wxColour &darkColor, const wxColour &lightColor, int squareSize,
	               wxWindow *parent, wxWindowID winId = wxID_ANY,
	               const wxPoint &pos = wxDefaultPosition);

	/**
	 * Creates a new ChessboardGrid using two-step construction
	 */
	bool Create(const wxColour &darkColor, const wxColour &lightColor, int squareSize,
	            wxWindow *parent, wxWindowID winId = wxID_ANY,
	            const wxPoint &pos = wxDefaultPosition);

	/**
	 * Updates the icons
	 * @param pcPawn PC pawn
	 * @param pcDame PC dame
	 * @param plPawn PL pawn
	 * @param plDame PL dame
	 */
	void updateIcons(const wxBitmap &pcPawn, const wxBitmap &pcDame, const wxBitmap &plPawn, const wxBitmap &plDame);

	/**
	 * Updates the disposition of the pieces in the chessboard and clears every border
	 * @param newDisposition The new disposition to apply
	 */
	void updateDisposition(const GameUtils::Disposition &newDisposition);

	/**
	 * Set a new border for the square located at an specific location
	 * @param index Position of the square
	 * @param border The new border
	 */
	void SetSquareBorder(int index, const wxPen &border);

	/**
	 * Clears the border of all 64 squares
	 */
	void ClearSquareBorders();

private:
	ChessboardGrid(const ChessboardGrid &); // prevents copy-constructor
	std::array<ChessboardSquare *, 64> chessboard{};

	wxBitmap m_pcPawn, m_pcDame, m_plPawn, m_plDame;
	GameUtils::MoveList moves; // list of moves the player can do

	int m_squareSize{};

	void OnItemMouseClicked(wxMouseEvent &evt);
};


#endif //ITALIAN_DRAUGHTS_CHESSBOARD_GRID_H
