// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2023  Nicola Revelant

#ifndef ITALIAN_DRAUGHTS_DRAUGHTS_MATCH_MANAGER_H
#define ITALIAN_DRAUGHTS_DRAUGHTS_MATCH_MANAGER_H

#include "ChessboardGrid/ChessboardGrid.h"
#include "GameUtils/GameUtils.h"

#define DEF_MIN_GD 1
#define DEF_MAX_GD 10
#define BORDER_WIDTH 3
#define THREAD_ID 1

/**
 * This class handle 1 or more matches using a specific ChessboardGrid
 */
class MatchManager {
public:
	static const int selectedNone = -1, minGD = DEF_MIN_GD, maxGD = DEF_MAX_GD;

	/**
	 * Enum used to specify why MatchManager has called the update callback
	 */
	enum UpdateType {
		TURN_PLAYER,
		TURN_PC,
		PLAYER_WON,
		PC_WON,
		ILLEGAL_SELECTION,
		ILLEGAL_MOVE
	};

	/**
	 * Starts a new match using a chessboard grid
	 * @param chessboard Chessboard grid used as interface between player and PC
	 * @param focusColor Color used to highlight selected piece
	 * @param possibleMoveColor Color used to highlight possible moves
	 */
	explicit MatchManager(ChessboardGrid *chessboard, const wxColour &focusColor,
	                      const wxColour &possibleMoveColor);

	virtual ~MatchManager();

	/**
	 * Set a update listener that will be called when:
	 * <ul>
	 *  <li>User/PC make a move</li>
	 *  <li>User/PC won</li>
	 *  <li>User tries to make an illegal move</li>
	 * </ul>
	 * @param listener Method of Class called on update, if nullptr it removes the current listener
	 */
	void setOnUpdateListener(const std::function<void(enum UpdateType updateType)> &listener);

	/**
	 * Reset match and change difficulty
	 * @param newDifficulty The new difficulty
	 * @return True if newDifficulty is between minGD and maxGD and no thread is in running
	 */
	bool changeDifficulty(int newDifficulty);

	/**
	 * @return Current difficulty
	 */
	int getDifficulty() const;

	/**
	 * Reset the current match
	 * @return False when the algorithm thread is running
	 */
	bool newMatch();

	/**
	 * @return True if the player have moved at least 1 piece, and the game's not over
	 */
	bool isPlaying() const;

private:
	MatchManager(const MatchManager &); // prevents copy-constructor
	GameUtils::Disposition m_disposition{};
	ChessboardGrid *chessboardGrid;
	wxPen focusBorder, possibleMoveBorder;
	GameUtils::MoveList moves{};
	bool m_isEnd, m_isPlaying, m_threadRunning;
	int gameDifficulty = minGD, selectedPos = selectedNone;

	std::function<void(enum UpdateType updateType)> m_onUpdate;

	void onChessboardSquareClick(wxMouseEvent &evt);

	void onThreadFinish(wxCommandEvent &evt);

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

	/**
	 * If onUpdate listener is set, it calls the listener with updateType
	 * @param updateType Update type
	 */
	void notifyUpdate(UpdateType updateType);
};

#endif //ITALIAN_DRAUGHTS_DRAUGHTS_MATCH_MANAGER_H
