#ifndef ITALIAN_DRAUGHTS_MINIMAX_THREAD_H
#define ITALIAN_DRAUGHTS_MINIMAX_THREAD_H

#include "wx/wx.h"
#include "../GameLogic/GameLogic.h"

class MinimaxThread : public wxThread {
public:
	MinimaxThread(wxEvtHandler *evtHandler, Chessboard &board, int gameDifficult, int id);

private:
	Chessboard &m_board;
	int m_gameDifficult, m_id;

	void *Entry() override;

protected:
	wxEvtHandler *m_evtHandler;
};


#endif //ITALIAN_DRAUGHTS_MINIMAX_THREAD_H
