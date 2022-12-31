#include "ChessboardGrid.h"

ChessboardGrid::ChessboardGrid(const wxColour &darkColor, const wxColour &lightColor,
                               const wxColour &focusColor, const wxColour &possibleMoveColor,
							   wxWindow *parent, wxWindowID winId,
							   const wxPoint &pos, const wxSize &size) : ChessboardGrid() {
	Create(darkColor, lightColor, focusColor, possibleMoveColor, parent, winId, pos, size);

}

ChessboardGrid::ChessboardGrid() : // bitmaps
		pcPawn(SYSTEM_CFG_PATH"images/pcPawn.png"), pcDame(SYSTEM_CFG_PATH"images/pcDame.png"),
		plPawn(SYSTEM_CFG_PATH"images/plPawn.png"), plDame(SYSTEM_CFG_PATH"images/plDame.png") {}

bool ChessboardGrid::Create(const wxColour &darkColor, const wxColour &lightColor,
                            const wxColour &focusColor, const wxColour &possibleMoveColor,
                            wxWindow *parent, wxWindowID winId,
                            const wxPoint &pos, const wxSize &size) {
	if (!wxPanel::Create(parent, winId, pos, size))
		return false;

	focusBorder.SetColour(focusColor);
	focusBorder.SetWidth(3);

	possibleMoveBorder.SetColour(possibleMoveColor);
	possibleMoveBorder.SetWidth(3);

	auto *gridSizer = new wxGridSizer(8, 8, 0, 0);
	//auto *chessboardGrid = new wxPanel(parent, wxID_ANY, pos, size);
	SetSizer(gridSizer);

	//const wxColour lightColor = resources.getColor("light", DEF_LIGHT_COLOR);
	//const wxColour darkColor = resources.getColor("dark", DEF_DARK_COLOR);
	SetBackgroundColour(lightColor);

	ChessboardSquare *square;
	for (int row = 0, i = 0, col; row < 8; row++) {
		for (col = 0; col < 8; col++, i++) {
			square = new ChessboardSquare(this, i);
			if (row % 2 == col % 2) {
				square->SetBackgroundColour(darkColor);
			} else {
				square->SetBackgroundColour(lightColor);
			}
			square->Bind(wxEVT_LEFT_UP, &ChessboardGrid::OnItemMouseClicked, this);
			gridSizer->Add(square);
			chessboard[i] = square;
		}
	}

	Layout();
	return true;
}

void ChessboardGrid::updateBoardAndIcons(ChessboardManager::Move *move) {
	board.updateBoard(move);
	ChessboardSquare sq{2};

	for (int i = 0; i < 64; i++) {
		// update icons
		switch (board.get(i)) {
			case ChessboardManager::EMPTY:
				chessboard[i]->SetBitmap(wxNullBitmap);
				break;
			case ChessboardManager::PC_PAWN:
				chessboard[i]->SetBitmap(pcPawn);
				break;
			case ChessboardManager::PC_DAME:
				chessboard[i]->SetBitmap(pcDame);
				break;
			case ChessboardManager::PL_PAWN:
				chessboard[i]->SetBitmap(plPawn);
				break;
			case ChessboardManager::PL_DAME:
				chessboard[i]->SetBitmap(plDame);
				break;
		}
	}

	Refresh();
}

void ChessboardGrid::OnItemMouseClicked(wxMouseEvent &) {

}
