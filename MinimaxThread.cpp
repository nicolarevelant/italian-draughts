#include "MinimaxThread.h"

MinimaxThread::MinimaxThread(wxEvtHandler *evtHandler, Chessboard &board, int gameDifficult, int id)
		: wxThread(wxTHREAD_DETACHED), m_board(board) {
	m_evtHandler = evtHandler;
	m_gameDifficult = gameDifficult;
	m_id = id;
}

void *MinimaxThread::Entry() {
	wxCommandEvent evt(wxEVT_MENU, m_id);
	evt.SetClientData(GameLogic::calculateBestMove(m_board, m_gameDifficult));
	wxPostEvent(m_evtHandler, evt);
	return nullptr;
}
