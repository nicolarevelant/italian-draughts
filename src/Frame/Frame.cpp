#include "Frame.h"
#include "../ChessboardGrid/ChessboardGrid.h"

Frame::Frame(const std::string &theme)
		: wxFrame(nullptr, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(
		squareSize * 8 + border * 2 + padding * 2 + 160,
		squareSize * 8 + border * 2 + padding * 2 + 51)),
		  resources(theme),

 {
	wxFrame::SetTitle("Italian Draughts");
	wxColour bgColor = resources.getColor("bg");

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
	menuFile->Append(NEW_MATCH, _("&New match"));
	menuFile->AppendSeparator();
	menuFile->Append(wxID_EXIT, _("&Exit"), _("Leave the game"));

	auto *menuSettings = new wxMenu;
	//menuSettings->Append(CHANGE_GD, resources.getString("app.menu.changeGD"));
	menuSettings->Append(CHANGE_GD, _("&Change difficulty"));

	auto *menuHelp = new wxMenu;
	menuHelp->Append(wxID_ABOUT, _("About Italian Draughts"), _("Open about dialog"));

	auto *menuBar = new wxMenuBar;
	menuBar->Append(menuFile, _("&File"));
	menuBar->Append(menuSettings, _("&Settings"));
	menuBar->Append(menuHelp, _("&Help"));

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
	chessboardPanel->SetBackgroundColour(resources.getColor("border"));
	new ChessboardGrid(chessboardPanel, wxID_ANY,
					   resources.getColor("dark", DEF_DARK_COLOR),
					   resources.getColor("light", DEF_LIGHT_COLOR),
					   wxPoint(border, border),
					   wxSize(gridSize, gridSize));

	return chessboardPanel;
}

void Frame::updateStatusText(const wxString &text) {
	SetStatusText(wxString::Format(_("%s | Difficulty: %d"),
	                               text.empty() ? _(m_pcTurn ? "Opponent's turn" : "Your turn") : text,
	                               gameDifficult));
}

void Frame::updateChessboard(ChessboardManager::Move *move) {
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

	ChessboardManager::PieceType value = board.get(newSelection);
	if (value != ChessboardManager::PL_PAWN && value != ChessboardManager::PL_DAME) {
		selectedPos = selectedNone;
		Refresh();
		return;
	}

	/*bool isValid = false;
	for (auto &move: moves) {
		if (move->disposition[newSelection] == ChessboardManager::EMPTY) {
			// there is at least 1 move with this piece
			isValid = true;
			break;
		}
	}*/
	auto emptyChecker = [newSelection](ChessboardManager::Move *move) {return move->disposition[newSelection] == ChessboardManager::EMPTY; };

	if (std::any_of(moves.begin(), moves.end(), emptyChecker)) {
		// is valid
		chessboard[newSelection]->SetBorder(focusBorder);

		// highlight the possible moves
		ChessboardManager::PieceType oldValue;
		for (ChessboardManager::Move *move: moves) {
			oldValue = move->disposition[newSelection];
			if (oldValue != ChessboardManager::EMPTY) {
				continue; // this ChessboardManager::Move doesn't move the current selected piece
			}

			// highlight the empty squares in the current disposition that are filled in 'move'
			for (int i = 0; i < 64; i++) {
				if (board.get(i) == ChessboardManager::EMPTY && move->disposition[i] != ChessboardManager::EMPTY) {
					chessboard[i]->SetBorder(possibleMoveBorder);
				}
			}
		}

		selectedPos = newSelection;
	} else {
		wxMessageDialog dialog(this, _("You can't make any moves with this piece"),
		                       _("Invalid selection"));
		dialog.ShowModal();
		selectedPos = selectedNone;
	}
	Refresh();
}

ChessboardManager::Move *Frame::findPlayerMove(int oldIndex, int newIndex) {
	ChessboardManager::PieceType oldValue, newValue;

	// iterates the possible moves
	for (ChessboardManager::Move *move: moves) {
		oldValue = move->disposition[oldIndex];
		if (oldValue != ChessboardManager::EMPTY)
			continue; // this is not the correct move

		// here only if this move change the old position that becomes empty
		newValue = move->disposition[newIndex];
		if (newValue == ChessboardManager::PL_PAWN || newValue == ChessboardManager::PL_DAME)
			return move; // this is the correct move from oldIndex to newIndex
	}

	return nullptr;
}

void Frame::deleteMoves() {
	for (ChessboardManager::Move *move: moves) {
		delete move;
	}
}

Frame::~Frame() {
	deleteMoves();
}
