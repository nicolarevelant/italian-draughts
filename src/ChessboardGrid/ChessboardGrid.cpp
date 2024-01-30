// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2023  Nicola Revelant

#include "ChessboardGrid.h"
#include "ProjectVariables.h"

ChessboardGrid::ChessboardGrid() = default;

ChessboardGrid::~ChessboardGrid() = default;

ChessboardGrid::ChessboardGrid(const ImageProviderCB &images, const ColorProviderCB &colors,
							   wxWindow *parent, wxWindowID winId, const wxPoint &pos) : ChessboardGrid() {
	if (!Create(images, colors, parent, winId, pos)) {
#ifdef DEBUG
		std::cerr << "Cannot create ChessboardGrid" << std::endl;
#endif
		throw std::exception();
	}
}

bool ChessboardGrid::Create(const ImageProviderCB &images, const ColorProviderCB &colors,
                            wxWindow *parent, wxWindowID winId, const wxPoint &pos) {
	if (!wxPanel::Create(parent, winId, pos))
		return false;

	mFirstDame = images("firstDame", wxNullBitmap);
	if (!mFirstDame.IsOk()) return false;
	auto size = mFirstDame.GetSize();
	mFirstPawn = images("firstPawn", wxNullBitmap);
	if (!mFirstPawn.IsOk() || mFirstPawn.GetSize() != size) return false;

	mSecondDame = images("secondDame", wxNullBitmap);
	if (!mSecondDame.IsOk() || mSecondDame.GetSize() != size) return false;

	mSecondPawn = images("secondPawn", wxNullBitmap);
	if (!mSecondPawn.IsOk() || mSecondPawn.GetSize() != size) return false;

	mSelected = images("selected", wxNullBitmap);
	if (!mSelected.IsOk() || mSelected.GetSize() != size) return false;

	mPossibleMove = images("possibleMove", wxNullBitmap);
	if (!mPossibleMove.IsOk() || mPossibleMove.GetSize() != size) return false;

	wxPanel::SetInitialSize(size * 8);

	auto *gridSizer = new wxGridSizer(8, 8, 0, 0);
	SetSizer(gridSizer);

	ChessboardSquare *square;
	for (int row = 0, i = 0, col; row < 8; row++) {
		for (col = 0; col < 8; col++, i++) {
			square = new ChessboardSquare(size.GetWidth(), this, i);
			if (row % 2 == col % 2) {
				square->SetBackgroundColour(colors("dark", DEF_DARK_COLOR));
			} else {
				square->SetBackgroundColour(colors("light", DEF_LIGHT_COLOR));
			}
			square->Bind(wxEVT_LEFT_UP, &ChessboardGrid::OnItemMouseClicked, this);
			gridSizer->Add(square);
			mChessboard[i] = square;
		}
	}

	Layout();

	Bind(wxEVT_MENU, &ChessboardGrid::endStateChange, this, ST_CHNG_ID);
	Bind(wxEVT_MENU, &ChessboardGrid::endSquareSelected, this, SQ_SEL_ID);
	Bind(wxEVT_MENU, &ChessboardGrid::endSquarePossibleMove, this, SQ_POSS_MOVE_ID);
	Bind(wxEVT_MENU, &ChessboardGrid::endSquareClear, this, SQ_CLEAR_ID);
	Bind(wxEVT_MENU, &ChessboardGrid::endUpdateDisposition, this, UP_DISP_ID);

	Bind(wxEVT_MENU, &ChessboardGrid::onThreadFinished, this, THREAD_FINISHED_ID);

	mMatchManager = new MatchManager();
	mMatchManager->addEventListener(this);
	mIsThreadRunning = false;

	return true;
}

// on another thread

void ChessboardGrid::onStateChange(MatchManager::StateChangeType type) {
	auto *evt = new wxCommandEvent(wxEVT_MENU, ST_CHNG_ID);
	evt->SetInt(type);
	QueueEvent(evt);
}

void ChessboardGrid::onSquareSelected(int index) {
	auto *evt = new wxCommandEvent(wxEVT_MENU, SQ_SEL_ID);
	evt->SetInt(index);
	QueueEvent(evt);
}

void ChessboardGrid::onSquarePossibleMove(int index) {
	auto *evt = new wxCommandEvent(wxEVT_MENU, SQ_POSS_MOVE_ID);
	evt->SetInt(index);
	QueueEvent(evt);
}

void ChessboardGrid::onSquareClear() {
	QueueEvent(new wxCommandEvent(wxEVT_MENU, SQ_CLEAR_ID));
}

void ChessboardGrid::onUpdateDisposition(GameUtils::Disposition *newDisposition, bool isPcFirstPlayer) {
	auto *evt = new wxCommandEvent(wxEVT_MENU, UP_DISP_ID);
	evt->SetClientData(newDisposition);
	evt->SetInt(isPcFirstPlayer ? 1 : 0);
	QueueEvent(evt);
}

// end on another thread

void ChessboardGrid::OnItemMouseClicked(wxMouseEvent &evt) {
	if (mIsThreadRunning) return;

	int currentPos = evt.GetId();
	auto *thread = new WorkerThread(this, mMatchManager, currentPos, 1);
	wxThreadError err = thread->Run();
	if (err == wxTHREAD_NO_ERROR)
		mIsThreadRunning = true;
	else {
		std::cerr << "Cannot execute thread" << std::endl;
	}
}

void ChessboardGrid::onThreadFinished(wxCommandEvent &) {
	mIsThreadRunning = false;
}

void ChessboardGrid::endStateChange(wxCommandEvent &evt) {
	MatchManager::StateChangeType stateChangeType = static_cast<MatchManager::StateChangeType>(evt.GetInt());
	if (mOnStateChange) mOnStateChange(stateChangeType);
}

void ChessboardGrid::endSquareSelected(wxCommandEvent &evt) {
	mChessboard[evt.GetInt()]->SetForegroundBitmap(mSelected);
	wxWindow::Refresh();
}

void ChessboardGrid::endSquarePossibleMove(wxCommandEvent &evt) {
	mChessboard[evt.GetInt()]->SetForegroundBitmap(mPossibleMove);
	wxWindow::Refresh();
}

void ChessboardGrid::endSquareClear(wxCommandEvent &) {
	for (int i = 0; i < 64; i++) {
		mChessboard[i]->SetForegroundBitmap();
	}
	wxWindow::Refresh();
}

void ChessboardGrid::endUpdateDisposition(wxCommandEvent &evt) {
	auto *disposition = static_cast<GameUtils::Disposition *>(evt.GetClientData());
	bool isPcFirstPlayer = evt.GetInt() == 1;
	for (int i = 0; i < 64; i++) {
		mChessboard[i]->SetForegroundBitmap();
		switch (disposition->at(i)) {
			case GameUtils::EMPTY:
				mChessboard[i]->SetBackgroundBitmap();
				break;
			case GameUtils::PC_PAWN:
				mChessboard[i]->SetBackgroundBitmap(isPcFirstPlayer ? mFirstPawn : mSecondPawn);
				break;
			case GameUtils::PC_DAME:
				mChessboard[i]->SetBackgroundBitmap(isPcFirstPlayer ? mFirstDame : mSecondDame);
				break;
			case GameUtils::PLAYER_PAWN:
				mChessboard[i]->SetBackgroundBitmap(isPcFirstPlayer ? mSecondPawn : mFirstPawn);
				break;
			case GameUtils::PLAYER_DAME:
				mChessboard[i]->SetBackgroundBitmap(isPcFirstPlayer ? mSecondDame : mFirstDame);
				break;
		}
	}
	wxWindow::Refresh();
}

void ChessboardGrid::setOnStateChangeCB(const StateChangeCB &listener) {
	mOnStateChange = listener;
}

/**
 * Reset the current match
 * @return False when the algorithm thread is running
 */
bool ChessboardGrid::newMatch() {
	if (mIsThreadRunning) return false;
	return mMatchManager->newMatch();
}

/**
 * Reset match and change difficulty
 * @param newDifficulty The new difficulty
 * @return True if newDifficulty is between minGD and maxGD and no thread is in running
 */
bool ChessboardGrid::changeDifficulty(int newDifficulty) {
	if (mIsThreadRunning) return false;
	return mMatchManager->changeDifficulty(newDifficulty);
}

/**
 * Flip first player between PC and player
 * @return
 */
bool ChessboardGrid::flipFirstPlayer() {
	if (mIsThreadRunning) return false;
	if (mMatchManager->flipFirstPlayer()) {
		return true;
	}
	return false;
}

/**
 * @return Current difficulty
 */
int ChessboardGrid::getDifficulty() const {
	return mMatchManager->getDifficulty();
}

/**
 * @return True if the player have moved at least 1 piece, and the game's not over
 */
bool ChessboardGrid::isPlaying() const {
	return mMatchManager->isPlaying();
}

ChessboardGrid::WorkerThread::WorkerThread(wxEvtHandler *evtHandler, MatchManager *matchManager, int index, int id) :
	wxThread(wxTHREAD_DETACHED) {
	mEvtHandler = evtHandler;
	mMatchManager = matchManager;
	mIndex = index;
	mThreadID = id;
}

void *ChessboardGrid::WorkerThread::Entry() {
	mMatchManager->onChessboardSquareClick(mIndex);
	wxQueueEvent(mEvtHandler, new wxCommandEvent(wxEVT_MENU, THREAD_FINISHED_ID));
	return nullptr;
}
