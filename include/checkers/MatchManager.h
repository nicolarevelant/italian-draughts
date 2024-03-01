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

#ifndef MATCH_MANAGER_H
#define MATCH_MANAGER_H

#include "checkers/GameUtils.h"
#include <functional>
#include <atomic>

#define DEF_MIN_GD 0
#define DEF_MAX_GD 12

/**
 * This class handles 1 or more matches
 * Note: non-const methods are not thread-safe
 *
 * You can instantiate 2 or more objects to manage multiple games concurrently.
 */
class MatchManager {
public:
	static const int selectedNone = -1, minGD = DEF_MIN_GD, maxGD = DEF_MAX_GD;

	enum State {
		TURN_PLAYER,
		TURN_PC,
		PLAYER_WON,
		PC_WON,
		ILLEGAL_SELECTION,
		ILLEGAL_MOVE
	};

	class EventListener {
		public:
		virtual void onStateChange(State type) = 0;
		virtual void onSquareSelected(int index) = 0;
		virtual void onSquarePossibleMove(int index) = 0;
		virtual void onSquareClear() = 0;

		/**
		 * Pointer ownership: caller
		 */
		virtual void onUpdateDisposition(const GameUtils::Disposition *newDisposition) = 0;
	};

	/**
	 * Initialize the manager but you need to call newMatch() to start the game
	 */
	explicit MatchManager() = default;

	virtual ~MatchManager();

	/**
	 * Add a event listener, the pointer must not be freed
	 * until the caller removes the listener with removeEventListener()
	 */
	void addEventListener(EventListener *listener);

	/**
	 * Remove a event listener
	 */
	void removeEventListener(EventListener *listener);

	/**
	 * Click on the specified square
	 * 0 <= index <= 63
	*/
	void squareClick(int index);

	/**
	 * Start a new match or reset the current match
	 * @return True if the specified difficulty is supported
	 */
	bool newMatch(int newDifficulty, bool isPcFirstPlayer);

	/**
	 * This method is thread safe
	 * @return Current difficulty
	 */
	int getDifficulty() const;

	/**
	 * This method is thread safe
	 * @return True if the game is started and is not over
	 */
	bool isPlaying() const;

	/**
	 * This method is thread safe
	 * @return True if the game is over
	 */
	bool isEnd() const;

private:
	MatchManager(const MatchManager &); // prevents copy-constructor

	GameUtils::Disposition mDisposition{};
	GameUtils::MoveList mMoves{};
	std::atomic<bool> mIsEnd = false, mIsPlaying = false;
	std::atomic<int> mGameDifficulty;
	int mSelectedPos = selectedNone;
	std::vector<EventListener *> mListeners;

	void changeState(State type);
	void selectSquare(int index);
	void makeSquarePossibleMove(int index);
	void clearSquares();
	void updateDisposition(GameUtils::Disposition *newDisposition);

	/**
	 * Start the game algorithm to make a move
	 */
	void makePCMove();

	/**
	 * Resets mDisposition to the default disposition of the chessboard
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
