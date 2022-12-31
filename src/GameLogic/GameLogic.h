#ifndef ITALIAN_DRAUGHTS_GAME_LOGIC_H
#define ITALIAN_DRAUGHTS_GAME_LOGIC_H

#include "../ChessboardManager/ChessboardManager.h"

class GameLogic {
public:
	static ChessboardManager::Move *calculateBestMove(const ChessboardManager &board, int depth);

private:
	static int minimax(const ChessboardManager::Move *start_move, int eaten, bool maximizing, int depth, int alpha, int beta);
};


#endif //ITALIAN_DRAUGHTS_GAME_LOGIC_H
