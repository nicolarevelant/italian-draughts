/*
    Copyright (C) 2023-2024  Nicola Revelant

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "GameUtils.h"

#include <algorithm>
#include <climits>
#include <random>
#include <vector>

GameUtils::MoveList GameUtils::findMoves(const Disposition &disposition, bool player) {
	MoveList moves;

	if (player) {
		for (int position = 0; position < 64; position++) {
			switch (disposition[position]) {
				case PLAYER_DAME:
					addMoveStep(moves, disposition, position, true, false, 0);
					addMoveStep(moves, disposition, position, true, true, 0);
					addMoveStep(moves, disposition, position, false, false, 0);
					addMoveStep(moves, disposition, position, false, true, 0);
					break;
				case PLAYER_PAWN:
					addMoveStep(moves, disposition, position, false, false, 0);
					addMoveStep(moves, disposition, position, false, true, 0);
					break;
				case EMPTY:
				case PC_PAWN:
				case PC_DAME:
					break;
			}
		}
	} else {
		for (int position = 0; position < 64; position++) {
			switch (disposition[position]) {
				case PC_DAME:
					addMoveStep(moves, disposition, position, false, false, 0);
					addMoveStep(moves, disposition, position, false, true, 0);
					addMoveStep(moves, disposition, position, true, false, 0);
					addMoveStep(moves, disposition, position, true, true, 0);
					break;
				case PC_PAWN:
					addMoveStep(moves, disposition, position, true, false, 0);
					addMoveStep(moves, disposition, position, true, true, 0);
					break;
				case EMPTY:
				case PLAYER_PAWN:
				case PLAYER_DAME:
					break;
			}
		}
	}

	// if there is a move with score > 0 then it deletes the non-eating moves
	for (Move *move: moves) {
		// if a pawn can eat something, moves that eat nothing are removed from the list
		if (move->eatenFromPawn) {
			MoveList new_moves;
			for (Move *int_move: moves) {
				if (int_move->score > 0) {
					new_moves.push_back(int_move);
				} else {
					delete int_move;
				}
			}
			return new_moves;
		}
	}

	return moves;
}

bool GameUtils::addMoveStep(MoveList &moves, const Disposition &disposition, int source_position, bool row_offset,
                            bool col_offset, int score) {
	// invalid move (out of bounds)
	if (source_position / 8 == (row_offset ? 7 : 0) || source_position % 8 == (col_offset ? 7 : 0))
		return false;

	int position = source_position + (row_offset ? 8 : -8) + (col_offset ? 1 : -1);
	PieceType source_value = disposition[source_position];
	bool isValid = true;

	int mid_value = disposition[position];
	if (mid_value == EMPTY) {
		// move without jump, only if the number of jumps equals 0 (first and last step of the move)
		if (score == 0) {
			Disposition copy;
			std::copy(disposition.begin(), disposition.end(), copy.begin());
			copy[source_position] = EMPTY;
			if (row_offset) {
				copy[position] = (position / 8 == 7 && source_value == PC_PAWN) ? PC_DAME : source_value;
			} else {
				copy[position] = (position / 8 == 0 && source_value == PLAYER_PAWN) ? PLAYER_DAME : source_value;
			}
			moves.push_back(new Move(copy, false, 0));
			return true;
		}
		return false;
	}

	int jump_position = position + (row_offset ? 8 : -8) + (col_offset ? 1 : -1);

	// invalid move with jump (out of bounds)
	if (position / 8 == (row_offset ? 7 : 0) || position % 8 == (col_offset ? 7 : 0) || disposition[jump_position] != EMPTY) {
		return false;
	}

	if (source_value == (row_offset ? PC_PAWN : PLAYER_PAWN)) {
		if (mid_value != (row_offset ? PLAYER_PAWN : PC_PAWN)) {
			return false; // white man only eat black man and vice-versa
		}

		// move with jump from pawn, check only in the same y direction
		Disposition copy;
		std::copy(disposition.begin(), disposition.end(), copy.begin());
		copy[source_position] = EMPTY;
		copy[position] = EMPTY;
		if (row_offset) {
			copy[jump_position] = (jump_position / 8 == 7) ? PC_DAME : PC_PAWN;
		} else {
			copy[jump_position] = (jump_position / 8 == 0) ? PLAYER_DAME : PLAYER_PAWN;
		}
		score += PAWN_SCORE;

		if (addMoveStep(moves, copy, jump_position, row_offset, false, score))
			isValid = false;

		if (addMoveStep(moves, copy, jump_position, row_offset, true, score))
			isValid = false;

		if (isValid)
			moves.push_back(new Move(copy, true, score));

		return true;
	}

	if (source_value == PC_DAME) {
		if (mid_value == PC_DAME || mid_value == PC_PAWN)
			return false;
	} else if (source_value == PLAYER_DAME) {
		if (mid_value == PLAYER_DAME || mid_value == PLAYER_PAWN)
			return false;
	}

	// move with jump from dame
	Disposition copy;
	std::copy(disposition.begin(), disposition.end(), copy.begin());
	copy[source_position] = EMPTY;
	copy[position] = EMPTY;
	copy[jump_position] = source_value;
	score += (mid_value == PC_DAME || mid_value == PLAYER_DAME ) ? DAME_SCORE : PAWN_SCORE;

	if (addMoveStep(moves, copy, jump_position, row_offset, false, score))
		isValid = false;

	if (addMoveStep(moves, copy, jump_position, row_offset, true, score))
		isValid = false;

	if (addMoveStep(moves, copy, jump_position, !row_offset, false, score))
		isValid = false;

	if (addMoveStep(moves, copy, jump_position, !row_offset, true, score))
		isValid = false;

	if (isValid)
		moves.push_back(new Move(copy, false, score));

	return true;
}

struct {
	bool operator()(GameUtils::Move *a, GameUtils::Move *b) const { return a->score < b->score; }
} sortAscending;

struct {
	bool operator()(GameUtils::Move *a, GameUtils::Move *b) const { return a->score > b->score; }
} sortDiscending;

GameUtils::Move *GameUtils::calculateBestMove(const GameUtils::Disposition &disposition, int depth) {
	if (depth < 0) return nullptr;

	GameUtils::Move *res_move = nullptr;
	int bestScore = INT_MIN;
	int alpha = INT_MIN, beta = INT_MAX;

	std::vector<GameUtils::Move *> moves = GameUtils::findMoves(disposition, false);
	std::shuffle(moves.begin(), moves.end(), std::random_device());
	std::sort(moves.begin(), moves.end(), sortAscending);
	for (GameUtils::Move *move: moves) {
		int score = minimax(move, move->score, false, depth, alpha, beta);
		if (score == INT_MAX) {
			bestScore = INT_MAX;
			res_move = move;
			break;
		}

		if (score > bestScore) {
			bestScore = score;
			res_move = move;
		}

		if (score > alpha) {
			alpha = score;
		}
	}

	if (moves.empty()) {
		return nullptr;
	} else if (res_move == nullptr) {
		// bestScore equals INT_MIN
		res_move = moves.front();
	}

	for (GameUtils::Move *move: moves) {
		if (move != res_move)
			delete move;
	}

	return res_move;
}

int GameUtils::minimax(const GameUtils::Move *start_move, int oldScore, bool maximizing, int depth, int alpha,
                           int beta) {
	if (depth == 0) return oldScore; // depth limit reached

	int bestScore, score;

	if (maximizing) {
		bestScore = INT_MIN;
		std::vector<GameUtils::Move *> moves = GameUtils::findMoves(start_move->disposition, false);
		std::sort(moves.begin(), moves.end(), sortAscending);
		for (GameUtils::Move *move: moves) {
			score = minimax(move, oldScore + move->score, false, depth - 1, alpha, beta);
			if (score > bestScore) {
				bestScore = score;

				if (score > alpha) {
					alpha = score;
					if (beta <= alpha)
						break; // ignore other moves because parent won't choose this path
				}
			}
		}

		for (GameUtils::Move *move: moves) {
			delete move;
		}
		return bestScore;
	}

	bestScore = INT_MAX;
	std::vector<GameUtils::Move *> moves = GameUtils::findMoves(start_move->disposition, true);
	std::sort(moves.begin(), moves.end(), sortDiscending);
	for (GameUtils::Move *move: moves) {
		score = minimax(move, oldScore - move->score, true, depth - 1, alpha, beta);
		if (score < bestScore) {
			bestScore = score;

			if (score < beta) {
				beta = score;
				if (beta <= alpha)
					break; // ignore other moves because parent won't choose this path
			}
		}
	}

	for (GameUtils::Move *move: moves) {
		delete move;
	}
	return bestScore;
}
