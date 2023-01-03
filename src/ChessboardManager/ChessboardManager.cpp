#include "ChessboardManager.h"
#include "utils.h"

ChessboardManager::ChessboardManager(ChessboardGrid *chessboard, const wxColour &focusColor, const wxColour &possibleMoveColor) {
	focusBorder.SetColour(focusColor);
	focusBorder.SetWidth(3);

	possibleMoveBorder.SetColour(possibleMoveColor);
	possibleMoveBorder.SetWidth(3);

	chessboard->Bind(wxEVT_LEFT_UP, &ChessboardManager::onChessboardSquareClick, this);
}

ChessboardManager::~ChessboardManager() = default;

void ChessboardManager::onChessboardSquareClick(wxMouseEvent &event) {
	int currentPos = event.GetId();
	std::cout << "Clicked pos: " << currentPos << std::endl;
}

MoveList ChessboardManager::findMoves(bool pcTurn) const {
	return GameManager::findMoves(m_disposition, pcTurn);
}

void ChessboardManager::updateBoard(Move *move) {
	if (move == nullptr) {
		setDefaultLayout();
	} else {
		for (int i = 0; i < 64; i++) {
			m_disposition[i] = move->disposition[i];
		}
	}
}

/**
 * Resets the initial pieces disposition
 */
void ChessboardManager::setDefaultLayout() {
	for (int i = 0; i < 64; i++) {
		if ((i / 8) % 2 == i % 2) {
			if (i < (8 * 3)) {
				m_disposition[i] = PC_PAWN;
			} else if (i >= (8 * 5)) {
				m_disposition[i] = PL_PAWN;
			} else {
				m_disposition[i] = EMPTY;
			}
		} else {
			m_disposition[i] = EMPTY;
		}
	}
}

PieceType ChessboardManager::get(int index) const {
	return m_disposition[index];
}
