// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2023  Nicola Revelant

#include "MatchManager.h"
#include <vector>
#include <unistd.h>

MatchManager::~MatchManager() {
	for (GameUtils::Move *move: mMoves) {
		delete move;
	}
}

void MatchManager::addEventListener(EventListener *listener) {
	mListeners.push_back(listener);
}

void MatchManager::removeEventListener(EventListener *listener) {
	std::erase(mListeners, listener);
}

void MatchManager::squareClick(int currentPos) {
	if (!mIsPlaying) return;

	if (mSelectedPos == selectedNone) {
		if ((mDisposition[currentPos] == GameUtils::PLAYER_PAWN ||
			mDisposition[currentPos] == GameUtils::PLAYER_DAME)) {

			if (highlightPossibleMoves(currentPos)) {
				selectSquare(currentPos);
				mSelectedPos = currentPos;
			} else {
				changeState(ILLEGAL_SELECTION);
			}
		}
		return;
	}

	// change selection
	if ((mDisposition[currentPos] == GameUtils::PLAYER_PAWN ||
		mDisposition[currentPos] == GameUtils::PLAYER_DAME)) {

		clearSquares(); // it clears selectedPos and possible moves
		if (currentPos == mSelectedPos) {
			mSelectedPos = selectedNone;
			return;
		}

		if (highlightPossibleMoves(currentPos)) {
			selectSquare(currentPos);
			mSelectedPos = currentPos;
		} else {
			mSelectedPos = selectedNone;
			changeState(ILLEGAL_SELECTION);
		}

		return;
	}

	// illegal selection, deselect the current selection
	if (mDisposition[currentPos] != GameUtils::EMPTY || currentPos % 2 != (currentPos / 8) % 2) {
		clearSquares(); // it clears selectedPos and possible moves
		mSelectedPos = selectedNone;
		return;
	}

	GameUtils::Move *move = findPlayerMove(mSelectedPos, currentPos);
	if (move == nullptr) {
		// illegal move
		clearSquares();
		mSelectedPos = selectedNone;
		changeState(ILLEGAL_MOVE);
		return;
	}

	// legal move
	mDisposition = move->disposition;
	updateDisposition(&mDisposition);
	mSelectedPos = selectedNone;

	makePCMove();
}

bool MatchManager::newMatch(int newDifficulty, bool isPcFirstPlayer) {
	if (newDifficulty < minGD || newDifficulty > maxGD)
		return false;
	mGameDifficulty = newDifficulty;

	setDefaultLayout();
	updateDisposition(&mDisposition);

	mIsEnd = false;
	mIsPlaying = true;
	if (isPcFirstPlayer) {
		makePCMove();
	} else {
		for (GameUtils::Move *move: mMoves) {
			delete move;
		}
		mMoves = GameUtils::findMoves(mDisposition, true);
		changeState(TURN_PLAYER);
	}

	return true;
}

int MatchManager::getDifficulty() const {
	return mGameDifficulty;
}

bool MatchManager::isPlaying() const {
	return mIsPlaying;
}

bool MatchManager::isEnd() const {
	return mIsEnd;
}

void MatchManager::changeState(State type) {
	for (auto &listener : mListeners) {
		listener->onStateChange(type);
	}
}

void MatchManager::selectSquare(int index) {
	for (auto &listener : mListeners) {
		listener->onSquareSelected(index);
	}
}

void MatchManager::makeSquarePossibleMove(int index) {
	for (auto &listener : mListeners) {
		listener->onSquarePossibleMove(index);
	}
}

void MatchManager::clearSquares() {
	for (auto &listener : mListeners) {
		listener->onSquareClear();
	}
}

void MatchManager::updateDisposition(GameUtils::Disposition *newDisposition) {
	for (auto &listener : mListeners) {
		listener->onUpdateDisposition(newDisposition);
	}
}

void MatchManager::makePCMove() {
	changeState(TURN_PC);
	sleep(5); // TODO: test delay
	auto *pcMove = GameUtils::calculateBestMove(mDisposition, mGameDifficulty);
	if (pcMove == nullptr) {
		// PC cannot do anything, player won
		mIsEnd = true;
		mIsPlaying = false;
		changeState(PLAYER_WON);
		return;
	}

	mDisposition = pcMove->disposition;
	updateDisposition(&mDisposition);
	delete pcMove;

	// deletes all moves before re-assignment
	for (GameUtils::Move *move: mMoves)
		delete move;

	mMoves = GameUtils::findMoves(mDisposition, true);
	if (mMoves.empty()) {
		// Player cannot do anything, PC won
		mIsEnd = true;
		mIsPlaying = false;
		changeState(PC_WON);
		return;
	}

	changeState(TURN_PLAYER);
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
				makeSquarePossibleMove(i);
			}
		}
	}

	return isValid;
}
