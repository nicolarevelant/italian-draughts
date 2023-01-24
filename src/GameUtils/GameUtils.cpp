// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2023  Nicola Revelant

#include "GameUtils.h"
#include "GameAlgorithm/GameAlgorithm.h"

GameUtils::AlgorithmThread::AlgorithmThread(wxEvtHandler *evtHandler, const Disposition &disposition, int gameDifficult,
                                            int id) : wxThread(wxTHREAD_DETACHED), m_disposition(disposition) {
	m_evtHandler = evtHandler;
	m_gameDifficult = gameDifficult;
	m_id = id;
}

void *GameUtils::AlgorithmThread::Entry() {
	wxCommandEvent evt(wxEVT_MENU, m_id);
	evt.SetClientData(GameAlgorithm::calculateBestMove(m_disposition, m_gameDifficult));
	m_evtHandler->AddPendingEvent(evt);

	return nullptr;
}

GameUtils::MoveList GameUtils::findMoves(const Disposition &disposition, bool player) {
	MoveList moves;

	if (player) {
		for (int row = 0, col; row < 8; row++) {
			for (col = 0; col < 8; col++) {
				switch (disposition[row * 8 + col]) {
					case PL_DAME:
						addMoveStep(moves, disposition, row, col, true, false, 0);
						addMoveStep(moves, disposition, row, col, true, true, 0);
						addMoveStep(moves, disposition, row, col, false, false, 0);
						addMoveStep(moves, disposition, row, col, false, true, 0);
						break;
					case PL_PAWN:
						addMoveStep(moves, disposition, row, col, false, false, 0);
						addMoveStep(moves, disposition, row, col, false, true, 0);
						break;
					case EMPTY:
					case PC_PAWN:
					case PC_DAME:
						break;
				}
			}
		}
	} else {
		for (int row = 0, col; row < 8; row++) {
			for (col = 0; col < 8; col++) {
				switch (disposition[row * 8 + col]) {
					case PC_DAME:
						addMoveStep(moves, disposition, row, col, false, false, 0);
						addMoveStep(moves, disposition, row, col, false, true, 0);
						addMoveStep(moves, disposition, row, col, true, false, 0);
						addMoveStep(moves, disposition, row, col, true, true, 0);
						break;
					case PC_PAWN:
						addMoveStep(moves, disposition, row, col, true, false, 0);
						addMoveStep(moves, disposition, row, col, true, true, 0);
						break;
					case EMPTY:
					case PL_PAWN:
					case PL_DAME:
						break;
				}
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

bool GameUtils::addMoveStep(MoveList &moves, const Disposition &disposition, int s_row, int s_col, bool row_offset,
                            bool col_offset, int score) {
	// invalid move (out of bounds)
	if (s_row == (row_offset ? 7 : 0) || s_col == (col_offset ? 7 : 0))
		return false;

	int row = s_row + (row_offset ? 1 : -1), col = s_col + (col_offset ? 1 : -1);
	PieceType s_value = disposition[s_row * 8 + s_col];
	bool isValid = true;

	int mid_value = disposition[row * 8 + col];
	if (mid_value == EMPTY) {
		// move without jump, only if the number of jumps equals 0 (first and last step of the move)
		if (score == 0) {
			Disposition copy;
			std::copy(disposition.begin(), disposition.end(), copy.begin());
			copy[s_row * 8 + s_col] = EMPTY;
			if (row_offset) {
				copy[row * 8 + col] = (row == 7 && s_value == PC_PAWN) ? PC_DAME : s_value;
			} else {
				copy[row * 8 + col] = (row == 0 && s_value == PL_PAWN) ? PL_DAME : s_value;
			}
			moves.push_back(new Move(copy, false, 0));
			return true;
		}
		return false;
	}

	if (row == (row_offset ? 7 : 0) || col == (col_offset ? 7 : 0)) {
		return false;
	}

	if (s_value == (row_offset ? PC_PAWN : PL_PAWN)) {
		if (mid_value != (row_offset ? PL_PAWN : PC_PAWN)) {
			return false; // white man only eat black man and vice-versa
		}

		// move with jump from pawn
		row += row_offset ? 1 : -1;
		col += col_offset ? 1 : -1;
		if (disposition[row * 8 + col] != EMPTY) {
			return false;
		}

		// possible jump, check only in the same y direction
		Disposition copy;
		std::copy(disposition.begin(), disposition.end(), copy.begin());
		copy[s_row * 8 + s_col] = EMPTY;
		copy[row * 8 + col + (row_offset ? -8 : 8) + (col_offset ? -1 : 1)] = EMPTY;
		if (row_offset) {
			copy[row * 8 + col] = (row == 7) ? PC_DAME : PC_PAWN;
		} else {
			copy[row * 8 + col] = (row == 0) ? PL_DAME : PL_PAWN;
		}
		score += PAWN_SCORE;

		if (addMoveStep(moves, copy, row, col, row_offset, false, score))
			isValid = false;

		if (addMoveStep(moves, copy, row, col, row_offset, true, score))
			isValid = false;

		if (isValid)
			moves.push_back(new Move(copy, true, score));

		return true;
	}
	if (s_value == PC_DAME) {
		if (mid_value == PC_DAME || mid_value == PC_PAWN)
			return false;
	}
	if (s_value == PL_DAME) {
		if (mid_value == PL_DAME || mid_value == PL_PAWN)
			return false;
	}

	// move with jump from dame
	row += row_offset ? 1 : -1;
	col += col_offset ? 1 : -1;
	if (disposition[row * 8 + col] != EMPTY) {
		return false;
	}

	// possible jump
	Disposition copy;
	std::copy(disposition.begin(), disposition.end(), copy.begin());
	copy[s_row * 8 + s_col] = EMPTY;
	copy[row * 8 + col + (row_offset ? -8 : 8) + (col_offset ? -1 : 1)] = EMPTY;
	copy[row * 8 + col] = s_value;
	score += DAME_SCORE;

	if (addMoveStep(moves, copy, row, col, row_offset, false, score))
		isValid = false;

	if (addMoveStep(moves, copy, row, col, row_offset, true, score))
		isValid = false;

	if (addMoveStep(moves, copy, row, col, !row_offset, false, score))
		isValid = false;

	if (addMoveStep(moves, copy, row, col, !row_offset, true, score))
		isValid = false;

	if (isValid)
		moves.push_back(new Move(copy, false, score));

	return true;
}
