// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2023  Nicola Revelant

#ifndef CHESSBOARD_GRID_H
#define CHESSBOARD_GRID_H

#include "ChessboardSquare/ChessboardSquare.h"
#include "MatchManager/MatchManager.h"
#include <wx/wx.h>
#include <functional>

#define ST_CHNG_ID 1
#define SQ_SEL_ID 2
#define SQ_POSS_MOVE_ID 3
#define SQ_CLEAR_ID 4
#define UP_DISP_ID 5

#define THREAD_FINISHED_ID 6

#define DEF_DARK_COLOR wxColour(32, 32, 32)
#define DEF_LIGHT_COLOR wxColour(140, 140, 140)

/**
 * The mission of this class is to provide a wxPanel composed by a 8x8 grid
 */
class ChessboardGrid : public wxPanel, private MatchManager::EventListener {
public:
	typedef std::function<const wxBitmap &(const std::string &, const wxBitmap &)> ImageProviderCB;
	typedef std::function<const wxColour &(const std::string &, const wxColour &)> ColorProviderCB;
	typedef std::function<void(enum MatchManager::StateChangeType type)> StateChangeCB;

	class WorkerThread : public wxThread {
	public:
		WorkerThread(wxEvtHandler *evtHandler, MatchManager *matchManager, int index, int id);

	private:
		MatchManager *mMatchManager;
		int mIndex, mThreadID;

		void *Entry() override;

	protected:
		wxEvtHandler *mEvtHandler;
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

	void onStateChange(enum MatchManager::StateChangeType type);
	void onSquareSelected(int index);
	void onSquarePossibleMove(int index);
	void onSquareClear();
	void onUpdateDisposition(GameUtils::Disposition *newDisposition, bool isPcFirstPlayer);

	void setOnStateChangeCB(const StateChangeCB &listener);

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
	ChessboardGrid(const ChessboardGrid &); // prevents copy-constructor
	std::array<ChessboardSquare *, 64> mChessboard{};

	wxBitmap mFirstPawn = wxNullBitmap, mFirstDame = wxNullBitmap, mSecondPawn = wxNullBitmap, mSecondDame = wxNullBitmap;
	wxBitmap mSelected = wxNullBitmap, mPossibleMove = wxNullBitmap;
	GameUtils::MoveList moves; // list of moves the player can do
	MatchManager *mMatchManager;
	StateChangeCB mOnStateChange;
	bool mIsThreadRunning;

	void OnItemMouseClicked(wxMouseEvent &evt);
	void onThreadFinished(wxCommandEvent &evt);

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
