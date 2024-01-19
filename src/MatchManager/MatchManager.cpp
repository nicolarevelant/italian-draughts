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

	mChessboardGrid = chessboard;
	mAlgorithmThread = nullptr;
	mIsPlaying = false;
	mIsEnd = false;
	mIsPcFirstPlayer = false;
}

MatchManager::~MatchManager() {
	for (GameUtils::Move *move: mMoves) {
		delete move;
	}
}

bool MatchManager::newMatch() {
	if (mAlgorithmThread) {
		mAlgorithmThread->Delete();
		mAlgorithmThread = nullptr;
	}

	setDefaultLayout();
	mChessboardGrid->updateDisposition(mDisposition, mIsPcFirstPlayer);

	// deletes all moves before re-assignment
	for (GameUtils::Move *move: mMoves)
		delete move;

	mIsEnd = false;
	if (mIsPcFirstPlayer) {
		mMoves.clear();
		makePCMove();
	} else {
		mIsPlaying = false;
		mMoves = GameUtils::findMoves(mDisposition, true);
		notifyUpdate(TURN_PLAYER);
	}

	return true;
}

void MatchManager::setOnUpdateListener(const UpdateCallback &updateCB) {
	mOnUpdate = updateCB;

	// if the game is not over notify the current state
	if (!mIsEnd) notifyUpdate(mAlgorithmThread ? TURN_PC : TURN_PLAYER);
}

bool MatchManager::changeDifficulty(int newDifficulty) {
	if (newDifficulty < minGD || newDifficulty > maxGD)
		return false;

	mGameDifficulty = newDifficulty;
	newMatch();
	return true;
}

bool MatchManager::flipFirstPlayer() {
	mIsPcFirstPlayer = !mIsPcFirstPlayer;
	newMatch();
	return true;
}

int MatchManager::getDifficulty() const {
	return mGameDifficulty;
}

bool MatchManager::isPlaying() const {
	return mIsPlaying;
}

void MatchManager::onChessboardSquareClick(wxMouseEvent &event) {
	if (mAlgorithmThread || mIsEnd) return;

	int currentPos = event.GetId();

	if (mSelectedPos == selectedNone) {
		if ((mDisposition[currentPos] == GameUtils::PLAYER_PAWN || mDisposition[currentPos] == GameUtils::PLAYER_DAME)) {
			if (highlightPossibleMoves(currentPos)) {
				mChessboardGrid->SetSquareSelectedOverlay(currentPos);
				mSelectedPos = currentPos;
			} else {
				notifyUpdate(ILLEGAL_SELECTION);
			}
		}
		return;
	}

	// change selection
	if ((mDisposition[currentPos] == GameUtils::PLAYER_PAWN || mDisposition[currentPos] == GameUtils::PLAYER_DAME)) {
		mChessboardGrid->ClearSquareOverlay(); // it clears selectedPos and possible moves
		if (currentPos == mSelectedPos) {
			mSelectedPos = selectedNone;
			return;
		}

		if (highlightPossibleMoves(currentPos)) {
			mChessboardGrid->SetSquareSelectedOverlay(currentPos);
			mSelectedPos = currentPos;
		} else {
			mSelectedPos = selectedNone;
			notifyUpdate(ILLEGAL_SELECTION);
		}

		return;
	}

	// illegal selection, deselect the current selection
	if (mDisposition[currentPos] != GameUtils::EMPTY || currentPos % 2 != (currentPos / 8) % 2) {
		mChessboardGrid->ClearSquareOverlay(); // it clears selectedPos and possible moves
		mSelectedPos = selectedNone;
		return;
	}

	GameUtils::Move *move = findPlayerMove(mSelectedPos, currentPos);
	if (move == nullptr) {
		// illegal move
		mChessboardGrid->ClearSquareOverlay();
		mSelectedPos = selectedNone;
		notifyUpdate(ILLEGAL_MOVE);
		return;
	}

	// legal move
	mChessboardGrid->updateDisposition(mDisposition = move->disposition, mIsPcFirstPlayer);
	mSelectedPos = selectedNone;

	makePCMove();
}

void MatchManager::makePCMove() {
	mIsPlaying = true;
	notifyUpdate(TURN_PC);
	mAlgorithmThread = new GameUtils::AlgorithmThread(mChessboardGrid, mDisposition, mGameDifficulty, THREAD_ID);
	if (mAlgorithmThread->Create() != wxTHREAD_NO_ERROR || mAlgorithmThread->Run() != wxTHREAD_NO_ERROR) {
		std::cerr << "Cannot execute thread" << std::endl;
		exit(1);
	}
}

void MatchManager::onThreadFinish(wxCommandEvent &evt) {
	if (!mAlgorithmThread) {
#ifdef DEBUG
		std::cerr << "Unwanted thread finished" << std::endl;
#endif
		delete static_cast<GameUtils::Move *>(evt.GetClientData());
		exit(1);
	}
	mAlgorithmThread = nullptr;

	auto *pcMove = static_cast<GameUtils::Move *>(evt.GetClientData());
	if (pcMove == nullptr) {
		// PC cannot do anything, player won
		mIsEnd = true;
		mIsPlaying = false;
		notifyUpdate(PLAYER_WON);
		return;
	}

	mChessboardGrid->updateDisposition(mDisposition = pcMove->disposition, mIsPcFirstPlayer);
	delete pcMove;

	// deletes all moves before re-assignment
	for (GameUtils::Move *move: mMoves)
		delete move;

	mMoves = GameUtils::findMoves(mDisposition, true);
	if (mMoves.empty()) {
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
				mDisposition[i] = GameUtils::PC_PAWN;
			} else if (i >= (8 * 5)) {
				mDisposition[i] = GameUtils::PLAYER_PAWN;
			} else {
				mDisposition[i] = GameUtils::EMPTY;
			}
		} else {
			mDisposition[i] = GameUtils::EMPTY;
		}
	}
}

GameUtils::Move *MatchManager::findPlayerMove(int oldIndex, int newIndex) {
	GameUtils::PieceType oldValue, newValue;

	// iterates the possible moves
	for (GameUtils::Move *move: mMoves) {
		oldValue = move->disposition[oldIndex];
		if (oldValue != GameUtils::EMPTY)
			continue; // this is not the correct move

		// here only if this move change the old position that becomes empty
		newValue = move->disposition[newIndex];
		if (newValue == GameUtils::PLAYER_PAWN || newValue == GameUtils::PLAYER_DAME)
			return move; // this is the correct move from oldIndex to newIndex
	}

	return nullptr;
}

bool MatchManager::highlightPossibleMoves(int from) {
	bool isValid = false;
	for (GameUtils::Move *move: mMoves) {
		if (move->disposition[from] != GameUtils::EMPTY) continue; // wrong move
		isValid = true;
		for (int i = 0; i < 64; i++) {
			if (mDisposition[i] == GameUtils::EMPTY && move->disposition[i] != GameUtils::EMPTY) {
				// possible move
				mChessboardGrid->SetSquarePossibleMoveOverlay(i);
			}
		}
	}

	return isValid;
}

void MatchManager::notifyUpdate(MatchManager::UpdateType updateType) {
	if (mOnUpdate != nullptr) mOnUpdate(updateType);
}
