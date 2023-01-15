// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2023  Nicola Revelant

#include <climits>
#include "GameAlgorithm.h"

GameUtils::Move *GameAlgorithm::calculateBestMove(const GameUtils::Disposition &disposition, int depth) {
	if (depth < 0) return nullptr;

	GameUtils::Move *res_move = nullptr;
	int bestScore = INT_MIN;
	int alpha = INT_MIN, beta = INT_MAX;

	std::vector<GameUtils::Move *> moves = GameUtils::findMoves(disposition, false);
	for (GameUtils::Move *move: moves) {
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

	for (GameUtils::Move *move: moves) {
		if (move != res_move)
			delete move;
	}

	return res_move;
}

int GameAlgorithm::minimax(const GameUtils::Move *start_move, int oldScore, bool maximizing, int depth, int alpha, int beta) {
	if (depth == 0) return oldScore; // depth limit reached

	int bestScore, score;

	if (maximizing) {
		bestScore = INT_MIN;
		std::vector<GameUtils::Move *> moves = GameUtils::findMoves(start_move->disposition, false);
		for (GameUtils::Move *move: moves) {
			score = minimax(move, oldScore + move->score, false, depth - 1, alpha, beta);
			if (score > bestScore) {
				bestScore = score;
			}
			if (score > alpha) {
				alpha = score;
				if (beta <= alpha)
					break; // ignore subtree
			}
		}

		for (GameUtils::Move *move: moves) {
			delete move;
		}
		return bestScore;
	}

	bestScore = INT_MAX;
	std::vector<GameUtils::Move *> moves = GameUtils::findMoves(start_move->disposition, true);
	for (GameUtils::Move *move: moves) {
		score = minimax(move, oldScore - move->score, true, depth - 1, alpha, beta);
		if (score < bestScore) {
			bestScore = score;
		}
		if (score < beta) {
			beta = score;
			if (beta <= alpha)
				break; // ignore subtree
		}
	}

	for (GameUtils::Move *move: moves) {
		delete move;
	}
	return bestScore;
}
