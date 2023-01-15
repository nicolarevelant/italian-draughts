// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2023  Nicola Revelant

#include "MatchManager.h"

MatchManager::MatchManager(ChessboardGrid *chessboard, const wxColour &focusColor,
                           const wxColour &possibleMoveColor) {
	if (chessboard == nullptr)
		throw std::exception();

	focusBorder.SetColour(focusColor);
	focusBorder.SetWidth(BORDER_WIDTH);

	possibleMoveBorder.SetColour(possibleMoveColor);
	possibleMoveBorder.SetWidth(BORDER_WIDTH);

	chessboard->Bind(wxEVT_LEFT_UP, &MatchManager::onChessboardSquareClick, this);
	chessboard->Bind(wxEVT_MENU, &MatchManager::onThreadFinish, this, THREAD_ID);

	chessboardGrid = chessboard;
	m_threadRunning = false;
	m_isPlaying = false;
	m_isEnd = false;
	newMatch();
	notifyUpdate(TURN_PLAYER);
}

MatchManager::~MatchManager() {
	for (GameUtils::Move *move: moves) {
		delete move;
	}
}

void MatchManager::setOnUpdateListener(const std::function<void(enum UpdateType)> &listener) {
	m_onUpdate = listener;

	// if the game is over it doesn't call the listener
	if (!m_isEnd) notifyUpdate(m_threadRunning ? TURN_PC : TURN_PLAYER);
}

bool MatchManager::changeDifficulty(int newDifficulty) {
	if (m_threadRunning) return false;

	if (newDifficulty < minGD || newDifficulty > maxGD)
		return false;

	gameDifficulty = newDifficulty;
	if (isPlaying() || m_isEnd) newMatch();
	notifyUpdate(TURN_PLAYER);
	return true;
}

int MatchManager::getDifficulty() const {
	return gameDifficulty;
}

bool MatchManager::newMatch() {
	if (m_threadRunning) return false;

	setDefaultLayout();
	chessboardGrid->updateDisposition(m_disposition);
	m_isEnd = false;
	m_isPlaying = false;

	// deletes all moves before re-assignment
	for (GameUtils::Move *move: moves)
		delete move;

	moves = GameUtils::findMoves(m_disposition, true);
	return true;
}

bool MatchManager::isPlaying() const {
	return m_isPlaying;
}

void MatchManager::onChessboardSquareClick(wxMouseEvent &event) {
	if (m_threadRunning) return;

	int currentPos = event.GetId();

	if (selectedPos == selectedNone) {
		if ((m_disposition[currentPos] == GameUtils::PL_PAWN || m_disposition[currentPos] == GameUtils::PL_DAME)) {
			if (highlightPossibleMoves(currentPos)) {
				chessboardGrid->SetSquareBorder(currentPos, focusBorder);
				selectedPos = currentPos;
			} else {
				notifyUpdate(ILLEGAL_SELECTION);
			}
		}
		return;
	}

	// change selection
	if ((m_disposition[currentPos] == GameUtils::PL_PAWN || m_disposition[currentPos] == GameUtils::PL_DAME)) {
		chessboardGrid->ClearSquareBorders(); // it clears selectedPos and possible moves
		if (currentPos == selectedPos) {
			selectedPos = selectedNone;
			return;
		}

		if (highlightPossibleMoves(currentPos)) {
			chessboardGrid->SetSquareBorder(currentPos, focusBorder);
			selectedPos = currentPos;
		} else {
			selectedPos = selectedNone;
			notifyUpdate(ILLEGAL_SELECTION);
		}

		return;
	}

	// illegal selection, deselect the current selection
	if (m_disposition[currentPos] != GameUtils::EMPTY || currentPos % 2 != (currentPos / 8) % 2) {
		chessboardGrid->ClearSquareBorders(); // it clears selectedPos and possible moves
		selectedPos = selectedNone;
		return;
	}

	GameUtils::Move *move = findPlayerMove(selectedPos, currentPos);
	if (move == nullptr) {
		// illegal move
		chessboardGrid->ClearSquareBorders();
		selectedPos = selectedNone;
		notifyUpdate(ILLEGAL_MOVE);
		return;
	}

	// legal move
	chessboardGrid->updateDisposition(m_disposition = move->disposition);
	selectedPos = selectedNone;

	auto *thread = new GameUtils::AlgorithmThread(chessboardGrid, m_disposition, gameDifficulty, THREAD_ID);
	if (thread->Create() != wxTHREAD_NO_ERROR || thread->Run() != wxTHREAD_NO_ERROR) {
		exit(1);
	}

	m_isPlaying = true;
	m_threadRunning = true;
	notifyUpdate(TURN_PC);
}

void MatchManager::onThreadFinish(wxCommandEvent &evt) {
	m_threadRunning = false;

	auto *pcMove = static_cast<GameUtils::Move *>(evt.GetClientData());
	if (pcMove == nullptr) {
		// PC cannot do anything, player won
		m_isEnd = true;
		m_isPlaying = false;
		notifyUpdate(PLAYER_WON);
		return;
	}

	chessboardGrid->updateDisposition(m_disposition = pcMove->disposition);
	delete pcMove;

	// deletes all moves before re-assignment
	for (GameUtils::Move *move: moves)
		delete move;

	moves = GameUtils::findMoves(m_disposition, true);
	if (moves.empty()) {
		// Player cannot do anything, PC won
		m_isEnd = true;
		m_isPlaying = false;
		notifyUpdate(PC_WON);
		return;
	}

	notifyUpdate(TURN_PLAYER);
}

void MatchManager::setDefaultLayout() {
	for (int i = 0; i < 64; i++) {
		if ((i / 8) % 2 == i % 2) {
			if (i < (8 * 3)) {
				m_disposition[i] = GameUtils::PC_PAWN;
			} else if (i >= (8 * 5)) {
				m_disposition[i] = GameUtils::PL_PAWN;
			} else {
				m_disposition[i] = GameUtils::EMPTY;
			}
		} else {
			m_disposition[i] = GameUtils::EMPTY;
		}
	}
}

GameUtils::Move *MatchManager::findPlayerMove(int oldIndex, int newIndex) {
	GameUtils::PieceType oldValue, newValue;

	// iterates the possible moves
	for (GameUtils::Move *move: moves) {
		oldValue = move->disposition[oldIndex];
		if (oldValue != GameUtils::EMPTY)
			continue; // this is not the correct move

		// here only if this move change the old position that becomes empty
		newValue = move->disposition[newIndex];
		if (newValue == GameUtils::PL_PAWN || newValue == GameUtils::PL_DAME)
			return move; // this is the correct move from oldIndex to newIndex
	}

	return nullptr;
}

bool MatchManager::highlightPossibleMoves(int from) {
	bool isValid = false;
	for (GameUtils::Move *move: moves) {
		if (move->disposition[from] != GameUtils::EMPTY) continue; // wrong move
		isValid = true;
		for (int i = 0; i < 64; i++) {
			if (m_disposition[i] == GameUtils::EMPTY && move->disposition[i] != GameUtils::EMPTY) {
				// possible move
				chessboardGrid->SetSquareBorder(i, possibleMoveBorder);
			}
		}
	}

	return isValid;
}

void MatchManager::notifyUpdate(MatchManager::UpdateType updateType) {
	if (m_onUpdate != nullptr) m_onUpdate(updateType);
}
