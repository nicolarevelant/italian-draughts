#ifndef ITALIAN_DRAUGHTS_CHESSBOARD_H
#define ITALIAN_DRAUGHTS_CHESSBOARD_H

#include <bits/stdc++.h>

class Chessboard {
public:
	Chessboard();

	virtual ~Chessboard();

	enum PieceType {
		EMPTY = 0, // no piece
		PC_PAWN,   // pc pawn
		PC_DAME,   // pc dame
		PL_PAWN,   // player pawn
		PL_DAME    // player dame
	};

	struct Move {
		Move(const std::array<PieceType, 64> disposition, bool eatenFromPawn, int score)
				: disposition(disposition), eatenFromPawn(eatenFromPawn), score(score) {}

		const std::array<PieceType, 64> disposition; // the disposition after the move
		const bool eatenFromPawn; // this is used to determine which moves to discard (a pawn must eat if it can)
		const int score; // number of pieces eaten
	};

	typedef std::vector<Move *> MoveList;

	static std::array<Chessboard::PieceType, 64> copyDisposition(std::array<PieceType, 64> from);

	PieceType get(int index) const;

	void updateBoard(Move *move);

	MoveList findMoves(bool pcTurn) const;

	static MoveList findMoves(const Move *start_move, bool pcTurn);

private:
	Chessboard(const Chessboard &); // prevents copy-constructor
	std::array<PieceType, 64> m_disposition{};

	void setDefaultLayout();

	static MoveList findMoves(std::array<PieceType, 64> disposition, bool pcTurn);

	static bool addMoveStep(MoveList &moves, std::array<PieceType, 64> disposition,
	                        int s_row, int s_col, bool row_offset, bool col_offset, int score);
};

#endif //ITALIAN_DRAUGHTS_CHESSBOARD_H
