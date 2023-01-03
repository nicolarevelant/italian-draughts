#include "ChessboardGrid.h"
#include "ProjectVariables.h"

ChessboardGrid::ChessboardGrid() = default;

ChessboardGrid::~ChessboardGrid() = default;

ChessboardGrid::ChessboardGrid(const wxColour &darkColor, const wxColour &lightColor,
							   wxWindow *parent, wxWindowID winId,
							   const wxPoint &pos, int squareSize) : ChessboardGrid() {
	Create(darkColor, lightColor, parent, winId, pos, squareSize);
}

bool ChessboardGrid::Create(const wxColour &darkColor, const wxColour &lightColor,
                            wxWindow *parent, wxWindowID winId,
                            const wxPoint &pos, int sqSize) {
	if (!wxPanel::Create(parent, winId, pos, wxSize{sqSize, sqSize}))
		return false;

	if (!pcPawn.LoadFile(SYSTEM_CFG_PATH"images/pcPawn.png") ||
		!pcDame.LoadFile(SYSTEM_CFG_PATH"images/pcDame.png") ||
		!plPawn.LoadFile(SYSTEM_CFG_PATH"images/plPawn.png") ||
		!plDame.LoadFile(SYSTEM_CFG_PATH"images/plDame.png"))
		return false;

	squareSize = sqSize;

	auto *gridSizer = new wxGridSizer(8, 8, 0, 0);
	//auto *chessboardGrid = new wxPanel(parent, wxID_ANY, pos, size);
	SetSizer(gridSizer);

	//const wxColour lightColor = resources.getColor("light", DEF_LIGHT_COLOR);
	//const wxColour darkColor = resources.getColor("dark", DEF_DARK_COLOR);
	SetBackgroundColour(lightColor);

	ChessboardSquare *square;
	for (int row = 0, i = 0, col; row < 8; row++) {
		for (col = 0; col < 8; col++, i++) {
			square = new ChessboardSquare(squareSize, this, i);
			if (row % 2 == col % 2) {
				square->SetBackgroundColour(darkColor);
			} else {
				square->SetBackgroundColour(lightColor);
			}
			//square->Bind(wxEVT_LEFT_UP, &ChessboardGrid::OnItemMouseClicked, this);
			gridSizer->Add(square);
			chessboard[i] = square;
		}
	}

	Layout();
	return true;
}

void ChessboardGrid::updateDisposition(Disposition &newDisposition) {
	for (int i = 0; i < 64; i++) {
		chessboard[i]->SetBorder(wxNullPen);
		switch (newDisposition[i]) {
			case PieceType::EMPTY:
				chessboard[i]->SetBitmap(wxNullBitmap);
				break;
			case PieceType::PC_PAWN:
				chessboard[i]->SetBitmap(pcPawn);
				break;
			case PieceType::PC_DAME:
				chessboard[i]->SetBitmap(pcDame);
				break;
			case PieceType::PL_PAWN:
				chessboard[i]->SetBitmap(plPawn);
				break;
			case PieceType::PL_DAME:
				chessboard[i]->SetBitmap(plDame);
				break;
		}
	}
}

void ChessboardGrid::setBorder(int index, wxPen &border) {
	chessboard[index]->SetBorder(border);
}
