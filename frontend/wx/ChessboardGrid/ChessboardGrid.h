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

#ifndef CHESSBOARD_GRID_H
#define CHESSBOARD_GRID_H

#include "ChessboardSquare/ChessboardSquare.h"
#include "checkers/MatchManager.h"
#include <wx/wx.h>
#include <functional>

#define ST_CHNG_EVT_ID 1
#define SQ_SEL_EVT_ID 2
#define SQ_POSS_MOVE_EVT_ID 3
#define SQ_CLEAR_EVT_ID 4
#define UP_DISP_EVT_ID 5

#define THREAD_FINISHED_EVT_ID 20

#define DEF_DARK_COLOR wxColour(32, 32, 32)
#define DEF_LIGHT_COLOR wxColour(140, 140, 140)

/**
 * The mission of this class is to provide a wxPanel composed by a 8x8 grid
 */
class ChessboardGrid : public wxPanel, private MatchManager::EventListener {
public:
	typedef std::function<const wxBitmap &(const std::string &, const wxBitmap &)> ImageProviderCB;
	typedef std::function<const wxColour &(const std::string &, const wxColour &)> ColorProviderCB;
	typedef std::function<void(enum MatchManager::State type)> StateChangeCB;

	class WorkerThread : public wxThread {
	public:
		WorkerThread(wxEvtHandler *evtHandler, MatchManager *matchManager,
			int sqClickIndex, int mGameDifficulty, bool isPcFirstPlayer, int id);

	private:
		MatchManager *mMatchManager;
		int mSqClickIndex, mGameDifficulty, mThreadID;
		bool mIsPcFirstPlayer;
		wxEvtHandler *mEvtHandler;

		void *Entry() override;
	};

	ChessboardGrid();

	~ChessboardGrid() override;

	/**
	 * Creates a new ChessboardGrid
	 * @param darkColor Color used in dark squares
	 * @param lightColor Color used in light squares
	 * @param squareSize Square size
	 * @param parent Parent of this wxPanel
	 * @param winId Window ID, or wxID_ANY
	 * @param pos Position relative to the parent
	 */
	ChessboardGrid(const ImageProviderCB &images, const ColorProviderCB &colors, wxWindow *parent,
	               wxWindowID winId = wxID_ANY, const wxPoint &pos = wxDefaultPosition);

	/**
	 * Creates a new ChessboardGrid using two-step construction
	 */
	bool Create(const ImageProviderCB &images, const ColorProviderCB &colors, wxWindow *parent,
	            wxWindowID winId = wxID_ANY, const wxPoint &pos = wxDefaultPosition);

	void setOnStateChangeCB(const StateChangeCB &listener);

	/**
	 * Reset the current match
	 * @return False when the algorithm thread is running
	 */
	bool newMatch(int gameDifficulty, bool isPcFirstPlayer);

	/**
	 * @return Current difficulty
	 */
	int getDifficulty() const;

	/**
	 * @return True if the game is started and is not over
	 */
	bool isPlaying() const;

private:
	ChessboardGrid(const ChessboardGrid &); // prevents copy-constructor
	std::array<ChessboardSquare *, 64> mChessboard{};

	wxBitmap mFirstPawn = wxNullBitmap, mFirstDame = wxNullBitmap, mSecondPawn = wxNullBitmap, mSecondDame = wxNullBitmap;
	wxBitmap mSelected = wxNullBitmap, mPossibleMove = wxNullBitmap;
	GameUtils::MoveList moves; // list of moves the player can do
	MatchManager *mMatchManager;
	StateChangeCB mOnStateChange;
	bool mIsThreadRunning, mIsPcFirstPlayer;

	void OnItemMouseClicked(wxMouseEvent &evt);
	void onThreadFinished(wxCommandEvent &evt);

	void onStateChange(MatchManager::State type);
	void onSquareSelected(int index);
	void onSquarePossibleMove(int index);
	void onSquareClear();
	void onUpdateDisposition(const GameUtils::Disposition *newDisposition);

	void endStateChange(wxCommandEvent &evt);

	/**
	 * Highlight the specified square as selected
	 * @param index Position of the square
	 */
	void endSquareSelected(wxCommandEvent &evt);

	/**
	 * Highlight the specified square as possible move
	 * @param index Position of the square
	 */
	void endSquarePossibleMove(wxCommandEvent &evt);

	/**
	 * Clears the foreground bitmap of all 64 squares
	 */
	void endSquareClear(wxCommandEvent &evt);

	/**
	 * Updates the disposition of the pieces in the chessboard and clears every border
	 * @param newDisposition The new disposition to apply
	 * @param pcIsFirstPlayer True if PC is the first player
	 */
	void endUpdateDisposition(wxCommandEvent &evt);
};


#endif // CHESSBOARD_GRID_H
