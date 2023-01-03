#include <climits>
#include "GameAlgorithm.h"
#include "GameManager/GameManager.h"

Move *GameAlgorithm::calculateBestMove(const Disposition &disposition, int depth) {
	if (depth < 0) return nullptr;

	Move *res_move = nullptr;
	int bestScore = INT_MIN;
	int alpha = INT_MIN, beta = INT_MAX;

	std::vector<Move *> moves = GameManager::findMoves(disposition, true);
	for (Move *move: moves) {
		int score = minimax(move, move->score, false, depth, alpha, beta);
		if (score > bestScore) {
			bestScore = score;
			res_move = move;
		}
		if (score > alpha) {
			alpha = score;
			if (beta <= alpha)
				break; // ignore subtree
		}
	}

	if (moves.empty()) {
		return nullptr;
	} else if (res_move == nullptr) {
		// bestScore equals INT_MIN
		//res_move = moves.at(0); // all moves have the minimum score
		res_move = moves.front();
	}

	for (Move *move: moves) {
		if (move != res_move)
			delete move;
	}

	return res_move;
}

int GameAlgorithm::minimax(const Move *start_move, int eaten, bool maximizing, int depth, int alpha, int beta) {
	if (depth == 0) return eaten; // depth limit reached

	int bestScore, score;

	if (maximizing) {
		bestScore = INT_MIN;
		std::vector<Move *> moves = GameManager::findMoves(start_move->disposition, true);
		for (Move *move: moves) {
			score = minimax(move, eaten + move->score, false, depth - 1, alpha, beta);
			if (score > bestScore) {
				bestScore = score;
			}
			if (score > alpha) {
				alpha = score;
				if (beta <= alpha)
					break; // ignore subtree
			}
		}

		for (Move *move: moves) {
			delete move;
		}
		return bestScore;
	}

	bestScore = INT_MAX;
	std::vector<Move *> moves = GameManager::findMoves(start_move->disposition, false);
	for (Move *move: moves) {
		score = minimax(move, eaten - move->score, true, depth - 1, alpha, beta);
		if (score < bestScore) {
			bestScore = score;
		}
		if (score < beta) {
			beta = score;
			if (beta <= alpha)
				break; // ignore subtree
		}
	}

	for (Move *move: moves) {
		delete move;
	}
	return bestScore;
}
