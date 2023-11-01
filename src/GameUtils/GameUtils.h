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
		Move(const Disposition disposition, bool eatenFromPawn, int score) : disposition(disposition),
		                                                                     eatenFromPawn(eatenFromPawn),
		                                                                     score(score) {}

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

private:
	GameUtils() = default;

	static bool addMoveStep(MoveList &moves, const Disposition &disposition,
	                        int source_position, bool row_offset, bool col_offset, int score);
};


#endif //ITALIAN_DRAUGHTS_MINIMAX_THREAD_H
