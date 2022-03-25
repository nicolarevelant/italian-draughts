// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2023  Nicola Revelant

#ifndef ITALIAN_DRAUGHTS_GAME_LOGIC_H
#define ITALIAN_DRAUGHTS_GAME_LOGIC_H

#include "GameUtils/GameUtils.h"

/**
 * This class represents the Minimax algorithm used to play italian draughts
 */
class GameAlgorithm {
public:
	/**
	 * Calculates the best move for the computer
	 * @param disposition Current pieces' disposition
	 * @param depth How many recursion levels are allowed
	 * @return A possible move for the computer, or nullptr
	 */
	static GameUtils::Move *calculateBestMove(const GameUtils::Disposition &disposition, int depth);

private:
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


#endif //ITALIAN_DRAUGHTS_GAME_LOGIC_H
