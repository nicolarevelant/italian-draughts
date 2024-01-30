// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2023  Nicola Revelant

#ifndef MATCH_MANAGER_H
#define MATCH_MANAGER_H

#include "GameUtils/GameUtils.h"
#include <functional>

#define DEF_MIN_GD 0
#define DEF_MAX_GD 12
#define DEFAULT_DIFFICULTY 3

/**
 * This class handle 1 or more matches using a specific ChessboardGrid
 */
class MatchManager {
public:
	static const int selectedNone = -1, minGD = DEF_MIN_GD, maxGD = DEF_MAX_GD;

	/**
	 * Enum used to specify why MatchManager has called the update callback
	 */
	enum StateChangeType {
		TURN_PLAYER,
		TURN_PC,
		PLAYER_WON,
		PC_WON,
		ILLEGAL_SELECTION,
		ILLEGAL_MOVE
	};

	class EventListener {
		public:
		virtual void onStateChange(enum StateChangeType type) = 0;
		virtual void onSquareSelected(int index) = 0;
		virtual void onSquarePossibleMove(int index) = 0;
		virtual void onSquareClear() = 0;
		virtual void onUpdateDisposition(GameUtils::Disposition *newDisposition, bool isPcFirstPlayer) = 0;
	};

	/**
	 * Starts a new match using a chessboard grid
	 * @param chessboard Chessboard grid used as interface between player and PC
	 * @param focusColor Color used to highlight selected piece
	 * @param possibleMoveColor Color used to highlight possible moves
	 */
	explicit MatchManager();

	virtual ~MatchManager();

	/*
	 * Add the event listener and notify a state change on the new listener
	*/
	bool addEventListener(EventListener *listener);
	bool removeEventListener(EventListener *listener);

	void onChessboardSquareClick(int index);

	/**
	 * Reset the current match
	 * @return False when the algorithm thread is running
	 */
	bool newMatch();

	/**
	 * Reset match and change difficulty
	 * @param newDifficulty The new difficulty
	 * @return True if newDifficulty is between minGD and maxGD and no thread is in running
	 */
	bool changeDifficulty(int newDifficulty);

	/**
	 * Flip first player between PC and player
	 * @return
	 */
	bool flipFirstPlayer();

	/**
	 * @return Current difficulty
	 */
	int getDifficulty() const;

	/**
	 * @return True if the player have moved at least 1 piece, and the game's not over
	 */
	bool isPlaying() const;

private:
	MatchManager(const MatchManager &); // prevents copy-constructor

	GameUtils::Disposition mDisposition{};
	GameUtils::MoveList mMoves{};
	bool mIsEnd, mIsPlaying, mIsPcFirstPlayer;
	int mGameDifficulty = DEFAULT_DIFFICULTY, mSelectedPos = selectedNone;
	std::vector<EventListener *> mListeners;

	void changeState(enum StateChangeType type);
	void selectSquare(int index);
	void makeSquarePossibleMove(int index);
	void clearSquares();
	void updateDisposition(GameUtils::Disposition *newDisposition, bool isPcFirstPlayer);

	/**
	 * Start the game algorithm to make a move
	 */
	void makePCMove();

	/**
	 * Resets m_disposition to the default disposition of the chessboard
	 */
	void setDefaultLayout();

	/**
	 * @param oldIndex Selected position
	 * @param newIndex Final position
	 * @return A move that moves a piece from oldIndex to newIndex, or nullptr
	 */
	GameUtils::Move *findPlayerMove(int oldIndex, int newIndex);

	/**
	 * Highlights the possible moves from the specified position
	 * @return True if you can make at least 1 move
	 */
	bool highlightPossibleMoves(int from);
};

#endif // MATCH_MANAGER_H
