// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2023  Nicola Revelant

#ifndef ITALIAN_DRAUGHTS_MINIMAX_THREAD_H
#define ITALIAN_DRAUGHTS_MINIMAX_THREAD_H

#include "wx/wx.h"

#include <array>
#include <vector>

#define PAWN_SCORE 1
#define DAME_SCORE 2

/**
 * This class provides some utilities, such as a thread for calculating the PC's move
 * and a static method to find all possible moves
 */
class GameUtils {
public:
	enum PieceType {
		EMPTY = 0,
		PC_PAWN,
		PC_DAME,
		PLAYER_PAWN,
		PLAYER_DAME
	};

	/**
	 * Pieces' disposition (64 PieceType)
	 */
	typedef std::array<PieceType, 64> Disposition;

	class AlgorithmThread : public wxThread {
	public:
		AlgorithmThread(wxEvtHandler *evtHandler, const Disposition &disposition, int gameDifficulty, int id);

	private:
		const Disposition &mDisposition;
		int mGameDifficulty, mThreadID;

		void *Entry() override;

	protected:
		wxEvtHandler *m_evtHandler;
	};

	struct Move {
		Move(const Disposition disposition, bool eatenFromPawn, int score) :
			disposition(disposition), eatenFromPawn(eatenFromPawn), score(score) {}

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
	 * Find what moves player can do
	 * @param disposition The current disposition
	 * @param player True if user, false if PC
	 * @return The possible moves
	 */
	static MoveList findMoves(const Disposition &disposition, bool player);

	/**
	 * Calculates the best move for the computer
	 * @param disposition Current pieces' disposition
	 * @param depth How many recursion levels are allowed
	 * @return A possible move for the computer, or nullptr
	 */
	static GameUtils::Move *calculateBestMove(const GameUtils::Disposition &disposition, int depth);

private:
	GameUtils() = default;

	/**
	 * Add a move step to find how long the move is
	 */
	static bool addMoveStep(MoveList &moves, const Disposition &disposition,
							int source_position, bool row_offset, bool col_offset, int score);

	/**
	 * Calculates the score of the best move
	 * @param start_move
	 * @param oldScore The current score
	 * @param maximizing True if PC
	 * @param depth How many levels of recursion to do
	 * @param alpha Used by alpha-beta pruning
	 * @param beta Used by alpha-beta pruning
	 * @return The score after the best move, or INT_MIN if maximizing, or INT_MAX otherwise
	 */
	static int minimax(const GameUtils::Move *start_move, int oldScore, bool maximizing, int depth, int alpha, int beta);
};

#endif // ITALIAN_DRAUGHTS_MINIMAX_THREAD_H
