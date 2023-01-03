#ifndef ITALIAN_DRAUGHTS_CHESSBOARD_MANAGER_H
#define ITALIAN_DRAUGHTS_CHESSBOARD_MANAGER_H

#include "ChessboardGrid/ChessboardGrid.h"
#include "GameManager/GameManager.h"
#include "utils.h"

/**
 * The mission of this class is to handle a match
 */
class ChessboardManager {
public:
	explicit ChessboardManager(ChessboardGrid *chessboard, const wxColour &focusColor, const wxColour &possibleMoveColor);

	virtual ~ChessboardManager();

	PieceType get(int index) const;

	void updateBoard(Move *move);

	MoveList findMoves(bool pcTurn) const;

private:
	ChessboardManager(const ChessboardManager &); // prevents copy-constructor
	Disposition m_disposition{};
	wxPen focusBorder, possibleMoveBorder;

	void onChessboardSquareClick(wxMouseEvent &event);

	void setDefaultLayout();
};

#endif //ITALIAN_DRAUGHTS_CHESSBOARD_MANAGER_H
