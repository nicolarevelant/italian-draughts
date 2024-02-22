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

	Bind(wxEVT_MENU, &ChessboardGrid::endStateChange, this, ST_CHNG_EVT_ID);
	Bind(wxEVT_MENU, &ChessboardGrid::endSquareSelected, this, SQ_SEL_EVT_ID);
	Bind(wxEVT_MENU, &ChessboardGrid::endSquarePossibleMove, this, SQ_POSS_MOVE_EVT_ID);
	Bind(wxEVT_MENU, &ChessboardGrid::endSquareClear, this, SQ_CLEAR_EVT_ID);
	Bind(wxEVT_MENU, &ChessboardGrid::endUpdateDisposition, this, UP_DISP_EVT_ID);

	Bind(wxEVT_MENU, &ChessboardGrid::onThreadFinished, this, THREAD_FINISHED_EVT_ID);

	mMatchManager = new MatchManager();
	mMatchManager->addEventListener(this);
	mIsThreadRunning = false;

	return true;
}

void ChessboardGrid::OnItemMouseClicked(wxMouseEvent &evt) {
	if (mIsThreadRunning) return;

	int currentPos = evt.GetId();
	auto *thread = new WorkerThread(this, mMatchManager, currentPos, -1, false, THREAD_FINISHED_EVT_ID);
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

// begin EventListener callbacks

void ChessboardGrid::onStateChange(MatchManager::State type) {
	auto *evt = new wxCommandEvent(wxEVT_MENU, ST_CHNG_EVT_ID);
	evt->SetInt(type);
	QueueEvent(evt);
}

void ChessboardGrid::onSquareSelected(int index) {
	auto *evt = new wxCommandEvent(wxEVT_MENU, SQ_SEL_EVT_ID);
	evt->SetInt(index);
	QueueEvent(evt);
}

void ChessboardGrid::onSquarePossibleMove(int index) {
	auto *evt = new wxCommandEvent(wxEVT_MENU, SQ_POSS_MOVE_EVT_ID);
	evt->SetInt(index);
	QueueEvent(evt);
}

void ChessboardGrid::onSquareClear() {
	QueueEvent(new wxCommandEvent(wxEVT_MENU, SQ_CLEAR_EVT_ID));
}

void ChessboardGrid::onUpdateDisposition(const GameUtils::Disposition *newDisposition) {
	auto *evt = new wxCommandEvent(wxEVT_MENU, UP_DISP_EVT_ID);
	auto *tmp = new GameUtils::Disposition();
	std::copy(newDisposition->begin(), newDisposition->end(), tmp->begin());
	evt->SetClientData(tmp);
	QueueEvent(evt);
}

// end EventListener callbacks

void ChessboardGrid::endStateChange(wxCommandEvent &evt) {
	MatchManager::State state = static_cast<MatchManager::State>(evt.GetInt());
	if (mOnStateChange) mOnStateChange(state);
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
	for (int i = 0; i < 64; i++) {
		mChessboard[i]->SetForegroundBitmap(); // clear square
		switch (disposition->at(i)) {
			case GameUtils::EMPTY:
				mChessboard[i]->SetBackgroundBitmap();
				break;
			case GameUtils::PC_PAWN:
				mChessboard[i]->SetBackgroundBitmap(mIsPcFirstPlayer ? mFirstPawn : mSecondPawn);
				break;
			case GameUtils::PC_DAME:
				mChessboard[i]->SetBackgroundBitmap(mIsPcFirstPlayer ? mFirstDame : mSecondDame);
				break;
			case GameUtils::PLAYER_PAWN:
				mChessboard[i]->SetBackgroundBitmap(mIsPcFirstPlayer ? mSecondPawn : mFirstPawn);
				break;
			case GameUtils::PLAYER_DAME:
				mChessboard[i]->SetBackgroundBitmap(mIsPcFirstPlayer ? mSecondDame : mFirstDame);
				break;
		}
	}
	delete disposition;
	wxWindow::Refresh();
}

void ChessboardGrid::setOnStateChangeCB(const StateChangeCB &listener) {
	mOnStateChange = listener;
}

bool ChessboardGrid::newMatch(int gameDifficulty, bool isPcFirstPlayer) {
	if (mIsThreadRunning) return false;

	mIsPcFirstPlayer = isPcFirstPlayer;

	auto *thread = new WorkerThread(this, mMatchManager, -1, gameDifficulty, isPcFirstPlayer, THREAD_FINISHED_EVT_ID);
	wxThreadError err = thread->Run();
	if (err == wxTHREAD_NO_ERROR)
		mIsThreadRunning = true;
	else {
		std::cerr << "Cannot execute thread" << std::endl;
	}

	return true;
}

int ChessboardGrid::getDifficulty() const {
	return mMatchManager->getDifficulty();
}

bool ChessboardGrid::isPlaying() const {
	return mMatchManager->isPlaying();
}

ChessboardGrid::WorkerThread::WorkerThread(wxEvtHandler *evtHandler, MatchManager *matchManager, int sqClickIndex, int difficulty, bool isPcFirstPlayer, int id) :
	wxThread(wxTHREAD_DETACHED) {
	mEvtHandler = evtHandler;
	mMatchManager = matchManager;
	mSqClickIndex = sqClickIndex;
	mGameDifficulty = difficulty;
	mIsPcFirstPlayer = isPcFirstPlayer;
	mThreadID = id;
}

void *ChessboardGrid::WorkerThread::Entry() {
	if (mGameDifficulty == -1) // TODO: fix workaround
		mMatchManager->squareClick(mSqClickIndex);
	else
		mMatchManager->newMatch(mGameDifficulty, mIsPcFirstPlayer);

	wxQueueEvent(mEvtHandler, new wxCommandEvent(wxEVT_MENU, mThreadID));
	return nullptr;
}
