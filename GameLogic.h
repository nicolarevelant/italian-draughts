#ifndef ITALIAN_DRAUGHTS_GAME_LOGIC_H
#define ITALIAN_DRAUGHTS_GAME_LOGIC_H

#include "Chessboard.h"

class GameLogic {
public:
	static Chessboard::Move *calculateBestMove(const Chessboard &board, int depth);

private:
	static int minimax(const Chessboard::Move *start_move, int eaten, bool maximizing, int depth, int alpha, int beta);
};


#endif //ITALIAN_DRAUGHTS_GAME_LOGIC_H
