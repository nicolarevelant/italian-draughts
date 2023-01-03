#include "Frame.h"

#include "wx/aboutdlg.h"

Frame::Frame() = default;

Frame::Frame(wxWindow *parent, const std::string &theme) : Frame() {
	Create(parent, theme);
}

bool Frame::Create(wxWindow *parent, const std::string &theme) {
	if (!wxFrame::Create(parent, wxID_ANY, "Italian Draughts", wxDefaultPosition, wxSize(
			squareSize * 8 + border * 2 + padding * 2 + 160,
			squareSize * 8 + border * 2 + padding * 2 + 51)))
		return false;

	const wxColour &bgColor = resources.getColor("bg");

	wxFrame::SetMenuBar(createMenuBar());
	wxStatusBar *statusBar = wxFrame::CreateStatusBar();
	statusBar->SetBackgroundColour(bgColor);
	//statusBar->SetForegroundColour(*wxWHITE);
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

	Bind(wxEVT_MENU, &Frame::onThreadFinished, this, THREAD_FINISH);
}

Frame::~Frame() {
	delete chessboardManager;
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
	auto *grid = new ChessboardGrid(resources.getColor("dark", DEF_DARK_COLOR),
					   resources.getColor("light", DEF_LIGHT_COLOR),
					   resources.getColor("focus-border"),
					   resources.getColor("possible-move-border"),
					   chessboardPanel, wxID_ANY,
					   wxPoint(border, border),
					   squareSize);
	chessboardManager = new ChessboardManager(grid);

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
		for (ChessboardManager::Move *move : moves) {
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



//                             --- Menu events

void Frame::newMatchClicked(wxCommandEvent &) {
	if (m_pcTurn) return;

	if (m_isPlaying) {
		wxMessageDialog dialog(this, _("Are you sure you want to leave the game?"), _("New match"), wxYES_NO);
		if (dialog.ShowModal() != wxID_YES) return;
		m_isPlaying = false;
	}

	m_isEnd = false;

	updateChessboard();
	updateStatusText();

	Refresh();
}

void Frame::closeFrame(wxCommandEvent &) {
	Close();
}

void Frame::changeDifficultyClicked(wxCommandEvent &) {
	if (m_pcTurn) {
		wxMessageDialog dialog(this, _("You have to wait your turn to continue..."), _("Wait your turn"));
		dialog.ShowModal();
		return;
	}

	if (m_isPlaying) {
		wxMessageDialog dialog(this, _("Are you sure you want to leave the game?"), _("New match"), wxYES_NO);
		if (dialog.ShowModal() != wxID_YES) return;
	}

	while (true) {
		wxString message = wxString::Format(_("Choose game difficulty from %d to %d"), minGD, maxGD);
		wxTextEntryDialog dialog(this, message, _("Game difficulty"));
		if (dialog.ShowModal() != wxID_OK) return;

		long value;
		if (dialog.GetValue().ToLong(&value) && value >= minGD && value <= maxGD) {
			m_isPlaying = false;
			m_isEnd = false;
			gameDifficult = (int) value;

			updateChessboard();
			updateStatusText();

			Refresh();
			return;
		}
	}
}

void Frame::aboutClicked(wxCommandEvent &) {
	wxAboutDialogInfo dialog;
	dialog.SetName(wxFrame::GetTitle());
	dialog.SetVersion(PROJECT_VERSION);
	dialog.SetDescription("Italian Draughts, a strategy game for 2 players.");
	dialog.SetCopyright("Copyright (C) 2022 Nicola Revelant");
	dialog.SetDevelopers(developers);
	dialog.SetLicense("GNU General Public License, version 3 or later");

	wxAboutBox(dialog, this);
}

//                             --- Other events

void Frame::OnItemMouseClicked(wxMouseEvent &event) {
	if (m_isEnd) return;
	if (m_pcTurn) {
		wxMessageDialog dialog(this, _("You have to wait your turn to continue..."), _("Wait your turn"));
		dialog.ShowModal();
		return;
	}
	int x, y;
	event.GetPosition(&x, &y);
	if (x < 0 || y < 0 || x >= squareSize || y >= squareSize) return; // illegal position

	int currentPos = event.GetId();

	if (selectedPos == selectedNone) {
		checkUpdateSelection(currentPos);
		return;
	}

	// already selected

	// same square or white square, illegal move
	if (currentPos == selectedPos || (currentPos / 8) % 2 != currentPos % 2) {
		checkUpdateSelection(); // it removes also the possible move borders
		return; // no move
	}

	ChessboardManager::PieceType value = board.get(currentPos);
	if (value != ChessboardManager::EMPTY) {
		checkUpdateSelection(currentPos);
		return; // no move
	}

	int oldSelection = selectedPos;
	checkUpdateSelection();
	ChessboardManager::Move *playerMove = findPlayerMove(oldSelection, currentPos);
	if (playerMove == nullptr) {
		Refresh();
		return; // illegal move
	}

	updateBoardAndIcons(playerMove);
	m_isPlaying = true;
	m_pcTurn = true;

	updateStatusText();
	auto *thread = new MinimaxThread(this, board, gameDifficult, THREAD_FINISH);
	if (thread->Create() != wxTHREAD_NO_ERROR || thread->Run() != wxTHREAD_NO_ERROR) {
		std::cerr << "Runtime error, cannot create a new thread, aborting..." << std::endl;
		exit(EXIT_FAILURE);
	}

	// The player move is in the moves vector that will be deleted after the PC move
	Refresh();
}

void Frame::onThreadFinished(wxCommandEvent &event) {
	m_pcTurn = false;
	auto *pcMove = reinterpret_cast<ChessboardManager::Move *>(event.GetClientData());
	if (pcMove == nullptr) {
		m_isEnd = true;
		m_isPlaying = false;
		updateStatusText(_("You won"));
		wxMessageDialog dialog(this, _("You won"), _("Game over"));
		dialog.ShowModal();
		return;
	}

	updateChessboard(pcMove);

	// updateChessboard also updates 'moves'
	if (moves.empty()) {
		m_isEnd = true;
		m_isPlaying = false;
		updateStatusText(_("You lost"));
		wxMessageDialog dialog(this, _("You lost"), _("Game over"));
		dialog.ShowModal();
	} else {
		updateStatusText();
	}
}

