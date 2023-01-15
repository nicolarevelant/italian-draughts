// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2023  Nicola Revelant

#include "Frame.h"
#include "wx/aboutdlg.h"

Frame::Frame() = default;

Frame::Frame(wxWindow *parent, const std::string &theme) : Frame() {
	if (!Create(parent, theme))
		throw std::exception();
}

bool Frame::Create(wxWindow *parent, const std::string &theme) {
	if (!wxFrame::Create(parent, wxID_ANY, PROJECT_PRETTY_NAME))
		return false;

	if (!theme.empty())
		resources.addTheme(theme);

	const wxColour &bgColor = resources.getColor("bg");
	const wxColour &fgColor = resources.getColor("fg");

	wxFrame::SetMenuBar(createMenuBar());
	wxStatusBar *statusBar = wxFrame::CreateStatusBar();
	statusBar->SetBackgroundColour(bgColor);
	statusBar->SetForegroundColour(fgColor);

	wxWindow::SetBackgroundColour(bgColor);
	wxWindow::SetForegroundColour(fgColor);

	// chessboard panel: board grid and border
	auto *chessboardPanel = createChessboard(this);

	auto chessboardSize = chessboardPanel->GetMinSize();
	SetClientSize(wxSize(chessboardSize.GetWidth() + CHESSBOARD_MARGIN_H * 2,
	                     chessboardSize.GetHeight() + CHESSBOARD_MARGIN_V * 2));

	// window sizer
	auto *mainSizer = new wxBoxSizer(wxHORIZONTAL);
	SetSizer(mainSizer);

	mainSizer->AddStretchSpacer();
	//mainSizer->Add(chessboardPanel, 0, wxALL, 20);
	mainSizer->Add(chessboardPanel, 0, wxALL, CHESSBOARD_MARGIN_V);
	mainSizer->AddStretchSpacer();

	return true;
}

Frame::~Frame() {
	delete chessboardManager;
}

wxMenuBar *Frame::createMenuBar() {
	auto *menuFile = new wxMenu;
	menuFile->Append(NEW_MATCH, _("&New match"), _("Start a new match"));
	menuFile->AppendSeparator();
	menuFile->Append(wxID_EXIT, _("&Exit"), _("Leave the game"));

	auto *menuSettings = new wxMenu;
	//menuSettings->Append(CHANGE_GD, resources.getString("app.menu.changeGD"));
	menuSettings->Append(CHANGE_GD, _("&Change difficulty"), _("Change difficulty"));

	auto *menuHelp = new wxMenu;
	menuHelp->Append(wxID_ABOUT, _("About " PROJECT_PRETTY_NAME), _("Open about dialog"));

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
	auto *chessboardPanel = new wxPanel(parent, wxID_ANY);
	chessboardPanel->SetBackgroundColour(resources.getColor("border"));
	auto *grid = new ChessboardGrid(resources.getColor("dark", DEF_DARK_COLOR),
	                                resources.getColor("light", DEF_LIGHT_COLOR),
	                                chessboardPanel, wxID_ANY,
	                                wxPoint(CHESSBOARD_BORDER_H, CHESSBOARD_BORDER_V));
	int width, height;
	grid->GetSize(&width, &height);
	chessboardPanel->SetMinSize(wxSize(width + CHESSBOARD_BORDER_H * 2, height + CHESSBOARD_BORDER_V * 2));

	chessboardManager = new MatchManager(grid,
	                                     resources.getColor("focus-border"),
	                                     resources.getColor("possible-move-border"));

	chessboardManager->setOnUpdateListener(std::bind(&Frame::onGameEvent, this, std::placeholders::_1));

	return chessboardPanel;
}

// Events

void Frame::onGameEvent(enum MatchManager::UpdateType updateType) {
	switch (updateType) {
		case MatchManager::TURN_PLAYER:
			SetStatusText(
					wxString::Format(_("%s | Difficulty: %d"), _("Your turn"), chessboardManager->getDifficulty()));
			break;
		case MatchManager::TURN_PC:
			SetStatusText(wxString::Format(_("%s | Difficulty: %d"), _("PC turn"), chessboardManager->getDifficulty()));
			break;
		case MatchManager::PLAYER_WON:
			SetStatusText(_("You won"));
			wxMessageDialog(this, _("You won"), _("Game over")).ShowModal();
			break;
		case MatchManager::PC_WON:
			SetStatusText(_("PC won"));
			wxMessageDialog(this, _("You lost"), _("Game over")).ShowModal();
			break;
		case MatchManager::ILLEGAL_SELECTION:
			wxMessageDialog(this, _("You can't make any moves with this piece"),
			                _("Invalid selection")).ShowModal();
			break;
		case MatchManager::ILLEGAL_MOVE:
			wxMessageDialog(this, _("You can't move the selected piece is this position"),
			                _("Invalid move")).ShowModal();
	}
}

void Frame::newMatchClicked(wxCommandEvent &) {
	if (chessboardManager->isPlaying()) {
		wxMessageDialog dialog(this, _("Are you sure you want to leave the game?"), _("New match"), wxYES_NO);
		if (dialog.ShowModal() != wxID_YES) return;
	}

	chessboardManager->newMatch();
}

void Frame::closeFrame(wxCommandEvent &) {
	Close();
}

void Frame::changeDifficultyClicked(wxCommandEvent &) {
	if (chessboardManager->isPlaying()) {
		wxMessageDialog dialog(this, _("Are you sure you want to leave the game?"), _("New match"), wxYES_NO);
		if (dialog.ShowModal() != wxID_YES) return;
	}

	while (true) {
		wxString message = wxString::Format(_("Choose game difficulty from %d to %d"), MatchManager::minGD,
		                                    MatchManager::maxGD);
		wxTextEntryDialog dialog(this, message, _("Game difficulty"));
		if (dialog.ShowModal() != wxID_OK) return;

		int value;
		if (dialog.GetValue().ToInt(&value) &&
		    value >= MatchManager::minGD && value <= MatchManager::maxGD &&
		    chessboardManager->changeDifficulty(value)) {
			return;
		}
	}
}

void Frame::aboutClicked(wxCommandEvent &) {
	wxAboutDialogInfo dialog;
	dialog.SetName(wxFrame::GetTitle());
	dialog.SetVersion(PROJECT_VERSION);
	dialog.SetDescription(PROJECT_DESCRIPTION);
	dialog.SetCopyright(PROJECT_COPYRIGHT);
	dialog.SetDevelopers(developers);
	dialog.SetLicense(PROJECT_LICENSE);

	wxAboutBox(dialog, this);
}
