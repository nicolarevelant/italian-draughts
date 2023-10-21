// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2023  Nicola Revelant

#include "MatchManager.h"

MatchManager::MatchManager(ChessboardGrid *chessboard) {
	if (chessboard == nullptr) {
#ifdef DEBUG
		std::cerr << "Cannot create MatchManager: chessboard == nullptr" << std::endl;
#endif
		throw std::exception();
	}

	chessboard->Bind(wxEVT_LEFT_UP, &MatchManager::onChessboardSquareClick, this);
	chessboard->Bind(wxEVT_MENU, &MatchManager::onThreadFinish, this, THREAD_ID);

	chessboardGrid = chessboard;
	algorithmThread = nullptr;
	mIsPlaying = false;
	mIsEnd = false;
	mIsPcFirstPlayer = false;
}

MatchManager::~MatchManager() {
	for (GameUtils::Move *move: moves) {
		delete move;
	}
}

bool MatchManager::newMatch() {
	if (algorithmThread) {
		algorithmThread->Delete();
		algorithmThread = nullptr;
	}

	setDefaultLayout();
	chessboardGrid->updateDisposition(m_disposition, mIsPcFirstPlayer);

	// deletes all moves before re-assignment
	for (GameUtils::Move *move: moves)
		delete move;

	mIsEnd = false;
	if (mIsPcFirstPlayer) {
		moves.clear();
		makePCMove();
	} else {
		mIsPlaying = false;
		moves = GameUtils::findMoves(m_disposition, true);
		notifyUpdate(TURN_PLAYER);
	}

	return true;
}

void MatchManager::setOnUpdateListener(const UpdateCallback &updateCB) {
	m_onUpdate = updateCB;

	// if the game is over it doesn't call the listener
	if (!mIsEnd) notifyUpdate(algorithmThread ? TURN_PC : TURN_PLAYER);
}

bool MatchManager::changeDifficulty(int newDifficulty) {
	if (newDifficulty < minGD || newDifficulty > maxGD)
		return false;

	gameDifficulty = newDifficulty;
	newMatch();
	return true;
}

bool MatchManager::flipFirstPlayer() {
	mIsPcFirstPlayer = !mIsPcFirstPlayer;
	newMatch();
	return true;
}

int MatchManager::getDifficulty() const {
	return gameDifficulty;
}

bool MatchManager::isPlaying() const {
	return mIsPlaying;
}

void MatchManager::onChessboardSquareClick(wxMouseEvent &event) {
	if (algorithmThread || mIsEnd) return;

	int currentPos = event.GetId();

	if (selectedPos == selectedNone) {
		if ((m_disposition[currentPos] == GameUtils::PL_PAWN || m_disposition[currentPos] == GameUtils::PL_DAME)) {
			if (highlightPossibleMoves(currentPos)) {
				chessboardGrid->SetSquareSelectedOverlay(currentPos);
				selectedPos = currentPos;
			} else {
				notifyUpdate(ILLEGAL_SELECTION);
			}
		}
		return;
	}

	// change selection
	if ((m_disposition[currentPos] == GameUtils::PL_PAWN || m_disposition[currentPos] == GameUtils::PL_DAME)) {
		chessboardGrid->ClearSquareOverlay(); // it clears selectedPos and possible moves
		if (currentPos == selectedPos) {
			selectedPos = selectedNone;
			return;
		}

		if (highlightPossibleMoves(currentPos)) {
			chessboardGrid->SetSquareSelectedOverlay(currentPos);
			selectedPos = currentPos;
		} else {
			selectedPos = selectedNone;
			notifyUpdate(ILLEGAL_SELECTION);
		}

		return;
	}

	// illegal selection, deselect the current selection
	if (m_disposition[currentPos] != GameUtils::EMPTY || currentPos % 2 != (currentPos / 8) % 2) {
		chessboardGrid->ClearSquareOverlay(); // it clears selectedPos and possible moves
		selectedPos = selectedNone;
		return;
	}

	GameUtils::Move *move = findPlayerMove(selectedPos, currentPos);
	if (move == nullptr) {
		// illegal move
		chessboardGrid->ClearSquareOverlay();
		selectedPos = selectedNone;
		notifyUpdate(ILLEGAL_MOVE);
		return;
	}

	// legal move
	chessboardGrid->updateDisposition(m_disposition = move->disposition, mIsPcFirstPlayer);
	selectedPos = selectedNone;

	makePCMove();
}

void MatchManager::makePCMove() {
	mIsPlaying = true;
	notifyUpdate(TURN_PC);
	algorithmThread = new GameUtils::AlgorithmThread(chessboardGrid, m_disposition, gameDifficulty, THREAD_ID);
	if (algorithmThread->Create() != wxTHREAD_NO_ERROR || algorithmThread->Run() != wxTHREAD_NO_ERROR) {
		std::cerr << "Cannot execute thread" << std::endl;
		exit(1);
	}
}

void MatchManager::onThreadFinish(wxCommandEvent &evt) {
	if (!algorithmThread) {
#ifdef DEBUG
		std::cerr << "Unwanted thread finished" << std::endl;
#endif
		delete static_cast<GameUtils::Move *>(evt.GetClientData());
		exit(1);
	}
	algorithmThread = nullptr;

	auto *pcMove = static_cast<GameUtils::Move *>(evt.GetClientData());
	if (pcMove == nullptr) {
		// PC cannot do anything, player won
		mIsEnd = true;
		mIsPlaying = false;
		notifyUpdate(PLAYER_WON);
		return;
	}

	chessboardGrid->updateDisposition(m_disposition = pcMove->disposition, mIsPcFirstPlayer);
	delete pcMove;

	// deletes all moves before re-assignment
	for (GameUtils::Move *move: moves)
		delete move;

	moves = GameUtils::findMoves(m_disposition, true);
	if (moves.empty()) {
		// Player cannot do anything, PC won
		mIsEnd = true;
		mIsPlaying = false;
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
				chessboardGrid->SetSquarePossibleMoveOverlay(i);
			}
		}
	}

	return isValid;
}

void MatchManager::notifyUpdate(MatchManager::UpdateType updateType) {
	if (m_onUpdate != nullptr) m_onUpdate(updateType);
}
