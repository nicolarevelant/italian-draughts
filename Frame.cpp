#include "Frame.h"

Frame::Frame(const std::string &locale, const std::string &theme)
		: wxFrame(nullptr, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(
		squareSize * 8 + border * 2 + padding * 2 + 160,
		squareSize * 8 + border * 2 + padding * 2 + 51)),
		  strings(locale), colors(theme),

		// colors and borders
		  focusBorder(colors.get("focus-border", *wxWHITE), 3),
		  possibleMoveBorder(colors.get("possible-move-border", *wxRED), 3),

		// bitmaps
		  pcPawn(SYSTEM_CFG_PATH"images/pcPawn.png"), pcDame(SYSTEM_CFG_PATH"images/pcDame.png"),
		  plPawn(SYSTEM_CFG_PATH"images/plPawn.png"), plDame(SYSTEM_CFG_PATH"images/plDame.png") {
	wxFrame::SetTitle(strings.get("app.title", PROJECT_NAME));
	wxColour bgColor = colors.get("bg");

	wxFrame::SetMenuBar(createMenuBar());
	wxStatusBar *statusBar = wxFrame::CreateStatusBar();
	statusBar->SetBackgroundColour(bgColor);
	statusBar->SetForegroundColour(*wxWHITE);
	updateStatusText();

	// window sizer
	auto *mainSizer = new wxBoxSizer(wxHORIZONTAL);
	SetSizer(mainSizer);
	wxWindow::SetBackgroundColour(bgColor);

	// chessboard panel: board grid and border
	auto *chessboardPanel = createChessboard(this);

	mainSizer->AddStretchSpacer();
	mainSizer->Add(chessboardPanel, 0, wxALL, 20);
	mainSizer->AddStretchSpacer();

	updateChessboard();

	Bind(wxEVT_MENU, &Frame::onThreadFinished, this, THREAD_FINISH);
}

wxMenuBar *Frame::createMenuBar() {
	auto *menuFile = new wxMenu;
	menuFile->Append(NEW_MATCH, strings["app.menu.new_match"]);
	menuFile->AppendSeparator();
	menuFile->Append(wxID_EXIT, strings["app.exit"], strings["app.exit.help"]);

	auto *menuSettings = new wxMenu;
	menuSettings->Append(CHANGE_GD, strings["app.menu.changeGD"]);

	auto *menuHelp = new wxMenu;
	menuHelp->Append(wxID_ABOUT, strings["app.about"], strings["app.about.help"]);

	auto *menuBar = new wxMenuBar;
	menuBar->Append(menuFile, strings["app.menu.file"]);
	menuBar->Append(menuSettings, strings["app.menu.settings"]);
	menuBar->Append(menuHelp, strings["app.menu.help"]);

	menuBar->Bind(wxEVT_MENU, &Frame::newMatchClicked, this, NEW_MATCH);
	menuBar->Bind(wxEVT_MENU, &Frame::closeFrame, this, wxID_EXIT);
	menuBar->Bind(wxEVT_MENU, &Frame::changeDifficultyClicked, this, CHANGE_GD);
	menuBar->Bind(wxEVT_MENU, &Frame::aboutClicked, this, wxID_ABOUT);

	return menuBar;
}

wxPanel *Frame::createChessboard(wxWindow *parent) {
	int gridSize = squareSize * 8;
	auto *chessboardPanel = new wxPanel(parent, wxID_ANY, wxDefaultPosition,
	                                    wxSize(gridSize + border * 2, gridSize + border * 2));
	chessboardPanel->SetBackgroundColour(colors["border"]);
	createChessboardGrid(chessboardPanel, wxPoint(border, border),
	                     wxSize(gridSize, gridSize));

	return chessboardPanel;
}

wxPanel *Frame::createChessboardGrid(wxWindow *parent, const wxPoint &pos, const wxSize &size) {
	auto *gridSizer = new wxGridSizer(8, 8, 0, 0);
	auto *chessboardGrid = new wxPanel(parent, wxID_ANY, pos, size);
	chessboardGrid->SetSizer(gridSizer);

	const wxColour lightColor = colors.get("light", DEF_LIGHT_COLOR);
	const wxColour darkColor = colors.get("dark", DEF_DARK_COLOR);
	chessboardGrid->SetBackgroundColour(lightColor);

	ChessboardSquare *square;
	for (int row = 0, i = 0, col; row < 8; row++) {
		for (col = 0; col < 8; col++, i++) {
			square = new ChessboardSquare(chessboardGrid, i);
			if (row % 2 == col % 2) {
				square->SetBackgroundColour(darkColor);
			} else {
				square->SetBackgroundColour(lightColor);
			}
			square->Bind(wxEVT_LEFT_UP, &Frame::OnItemMouseClicked, this);
			gridSizer->Add(square);
			chessboard[i] = square;
		}
	}

	chessboardGrid->Layout();
	return chessboardGrid;
}

void Frame::updateStatusText(const wxString &text) {
	SetStatusText(wxString::Format(strings["app.statusbar.ph"],
	                               text.empty() ? strings[m_pcTurn ? "game.turn.pc" : "game.turn.you"] : text,
	                               gameDifficult));
}

void Frame::updateBoardAndIcons(Chessboard::Move *move) {
	board.updateBoard(move);
	for (int i = 0; i < 64; i++) {
		// update icons
		switch (board.get(i)) {
			case Chessboard::EMPTY:
				chessboard[i]->SetBitmap(wxNullBitmap);
				break;
			case Chessboard::PC_PAWN:
				chessboard[i]->SetBitmap(pcPawn);
				break;
			case Chessboard::PC_DAME:
				chessboard[i]->SetBitmap(pcDame);
				break;
			case Chessboard::PL_PAWN:
				chessboard[i]->SetBitmap(plPawn);
				break;
			case Chessboard::PL_DAME:
				chessboard[i]->SetBitmap(plDame);
				break;
		}
	}

	Refresh();
}

void Frame::updateChessboard(Chessboard::Move *move) {
	updateBoardAndIcons(move);
	delete move;

	checkUpdateSelection();

	deleteMoves();
	moves = board.findMoves(false);
}

void Frame::checkUpdateSelection(int newSelection) {
	// it removes the border from all squares
	for (auto &i: chessboard) {
		i->SetBorder(wxNullPen);
	}

	if (newSelection == selectedNone) {
		selectedPos = newSelection;
		Refresh();
		return;
	}

	Chessboard::PieceType value = board.get(newSelection);
	if (value != Chessboard::PL_PAWN && value != Chessboard::PL_DAME) {
		selectedPos = selectedNone;
		Refresh();
		return;
	}

	/*bool isValid = false;
	for (auto &move: moves) {
		if (move->disposition[newSelection] == Chessboard::EMPTY) {
			// there is at least 1 move with this piece
			isValid = true;
			break;
		}
	}*/
	auto emptyChecker = [newSelection](Chessboard::Move *move) {return move->disposition[newSelection] == Chessboard::EMPTY; };

	if (std::any_of(moves.begin(), moves.end(), emptyChecker)) {
		// is valid
		chessboard[newSelection]->SetBorder(focusBorder);

		// highlight the possible moves
		Chessboard::PieceType oldValue;
		for (Chessboard::Move *move: moves) {
			oldValue = move->disposition[newSelection];
			if (oldValue != Chessboard::EMPTY) {
				continue; // this Chessboard::Move doesn't move the current selected piece
			}

			// highlight the empty squares in the current disposition that are filled in 'move'
			for (int i = 0; i < 64; i++) {
				if (board.get(i) == Chessboard::EMPTY && move->disposition[i] != Chessboard::EMPTY) {
					chessboard[i]->SetBorder(possibleMoveBorder);
				}
			}
		}

		selectedPos = newSelection;
	} else {
		wxMessageDialog dialog(this, strings["game.invalid-move.text"],
		                       strings["game.invalid-move"]);
		dialog.ShowModal();
		selectedPos = selectedNone;
	}
	Refresh();
}

Chessboard::Move *Frame::findPlayerMove(int oldIndex, int newIndex) {
	Chessboard::PieceType oldValue, newValue;

	// iterates the possible moves
	for (Chessboard::Move *move: moves) {
		oldValue = move->disposition[oldIndex];
		if (oldValue != Chessboard::EMPTY)
			continue; // this is not the correct move

		// here only if this move change the old position that becomes empty
		newValue = move->disposition[newIndex];
		if (newValue == Chessboard::PL_PAWN || newValue == Chessboard::PL_DAME)
			return move; // this is the correct move from oldIndex to newIndex
	}

	return nullptr;
}

void Frame::deleteMoves() {
	for (Chessboard::Move *move: moves) {
		delete move;
	}
}

Frame::~Frame() {
	deleteMoves();
}
