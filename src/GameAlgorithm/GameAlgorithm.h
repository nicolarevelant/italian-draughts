#ifndef ITALIAN_DRAUGHTS_GAME_LOGIC_H
#define ITALIAN_DRAUGHTS_GAME_LOGIC_H

#include "utils.h"

class GameAlgorithm {
public:
	/**
	 * Calculates the best move for the computer
	 * @param disposition Current pieces' disposition
	 * @param depth How many recursion levels are allowed
	 * @return A possible move for the computer, or nullptr
	 */
	static Move *calculateBestMove(const Disposition &disposition, int depth);

private:
	static int minimax(const Move *start_move, int eaten, bool maximizing, int depth, int alpha, int beta);
};


#endif //ITALIAN_DRAUGHTS_GAME_LOGIC_H
