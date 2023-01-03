#ifndef ITALIAN_DRAUGHTS_MINIMAX_THREAD_H
#define ITALIAN_DRAUGHTS_MINIMAX_THREAD_H

#include "wx/wx.h"

/**
 * The mission of this class is
 */
class GameManager {
public:
	class MinimaxThread : public wxThread {
	public:
		MinimaxThread(wxEvtHandler *evtHandler, const Disposition &disposition, int gameDifficult, int id);

	private:
		const Disposition &m_disposition;
		int m_gameDifficult, m_id;

		void *Entry() override;

	protected:
		wxEvtHandler *m_evtHandler;
	};

	static MoveList findMoves(const Disposition &disposition, bool pcTurn);

private:
	GameManager() = default;

	static bool addMoveStep(MoveList &moves, const Disposition &disposition,
	                        int s_row, int s_col, bool row_offset, bool col_offset, int score);
};




#endif //ITALIAN_DRAUGHTS_MINIMAX_THREAD_H
