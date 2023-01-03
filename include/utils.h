#ifndef ITALIAN_DRAUGHTS_UTILS_H
#define ITALIAN_DRAUGHTS_UTILS_H

#include <array>
#include <vector>

enum PieceType {
	EMPTY = 0, // no piece
	PC_PAWN,   // pc pawn
	PC_DAME,   // pc dame
	PL_PAWN,   // player pawn
	PL_DAME    // player dame
};

/**
 * Pieces' disposition (64 PieceType)
 */
typedef std::array<PieceType, 64> Disposition;

struct Move {
	Move(const Disposition disposition, bool eatenFromPawn, int score)
	: disposition(disposition), eatenFromPawn(eatenFromPawn), score(score) {}

	/**
	 * The disposition after the move
	 */
	const Disposition disposition;

	/**
	 * This is used to determine which moves to discard (a pawn must eat if it can)
	 */
	const bool eatenFromPawn;

	/**
	 * Score associated to the move
	 */
	const int score;
};

/**
 * Dynamic list of Move pointers
 */
typedef std::vector<Move *> MoveList;

/**
 * Copy a disposition and returns the new one
 * @param from The original disposition
 * @return The new disposition
 */
static Disposition copyDisposition(const Disposition &from) {
	Disposition to{};
	std::copy(from.begin(), from.end(), to.begin());

	return to;
}

#endif //ITALIAN_DRAUGHTS_UTILS_H
