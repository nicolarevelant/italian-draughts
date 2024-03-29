/*
    Copyright (C) 2023-2024  Nicola Revelant

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "Frame.h"
#include <functional>
#include <wx/aboutdlg.h>
#include "config.h"
#include "logo.xpm"

Frame::Frame() : resources(DATA_PATH) {}

Frame::Frame(wxWindow *parent, const std::string &theme) : Frame() {
	if (!Create(parent, theme)) {
#ifdef DEBUG
		std::cerr << "Cannot create Frame" << std::endl;
#endif
		throw std::exception();
	}
}

bool Frame::Create(wxWindow *parent, const std::string &theme) {
	if (!wxFrame::Create(parent, wxID_ANY, PROJECT_PRETTY_NAME))
		return false;

	if (!theme.empty())
		resources.addTheme(DATA_PATH, theme);

	// create chessboard panel that contains the chessboard grid
	auto *chessboardPanel = createChessboard(this);
	if (!chessboardPanel) {
		return false;
	}

	// window sizer
	auto *mainSizer = new wxBoxSizer(wxHORIZONTAL);
	mainSizer->AddStretchSpacer();
	mainSizer->Add(chessboardPanel, 0, wxALL, CHESSBOARD_MARGIN_V);
	mainSizer->AddStretchSpacer();
	SetSizer(mainSizer);

	// Menu Bar
	wxFrame::SetMenuBar(createMenuBar());

	const wxColour &bgColor = resources.getColor("bg");
	const wxColour &fgColor = resources.getColor("fg");

	// Status Bar
	wxStatusBar *statusBar = wxFrame::CreateStatusBar();
	statusBar->SetBackgroundColour(bgColor);
	statusBar->SetForegroundColour(fgColor);

	// Window theme
	wxWindow::SetBackgroundColour(bgColor);
	wxWindow::SetForegroundColour(fgColor);

	auto chessboardSize = chessboardPanel->GetMinSize();
	SetClientSize(wxSize(chessboardSize.GetWidth() + CHESSBOARD_MARGIN_H * 2,
	                     chessboardSize.GetHeight() + CHESSBOARD_MARGIN_V * 2));

	grid->setOnStateChangeCB(std::bind(&Frame::onGameEvent, this, std::placeholders::_1));

	mGameDifficulty = 3;
	mIsPcFirstPlayer = false;
	grid->newMatch(mGameDifficulty, mIsPcFirstPlayer);

	return true;
}

wxMenuBar *Frame::createMenuBar() {
	auto *menuFile = new wxMenu;
	menuFile->Append(NEW_MATCH, _("&New match"), _("Start a new match"));
	menuFile->AppendSeparator();
	menuFile->Append(wxID_EXIT, _("&Exit"), _("Leave the game"));

	auto *menuSettings = new wxMenu;
	menuSettings->Append(CHANGE_GD, _("Change &difficulty"), _("Change difficulty"));
	menuSettings->Append(TOGGLE_FIRST_PLAYER, _("&Toggle first player"), _("Toggle first player"));

	auto *menuHelp = new wxMenu;
	menuHelp->Append(wxID_ABOUT, _("About " PROJECT_PRETTY_NAME), _("Open about dialog"));

	auto *menuBar = new wxMenuBar;
	menuBar->Append(menuFile, _("&File"));
	menuBar->Append(menuSettings, _("&Settings"));
	menuBar->Append(menuHelp, _("&Help"));

	menuBar->Bind(wxEVT_MENU, &Frame::newMatchClicked, this, NEW_MATCH);
	menuBar->Bind(wxEVT_MENU, &Frame::closeFrame, this, wxID_EXIT);
	menuBar->Bind(wxEVT_MENU, &Frame::changeDifficultyClicked, this, CHANGE_GD);
	menuBar->Bind(wxEVT_MENU, &Frame::flipFirstPlayer, this, TOGGLE_FIRST_PLAYER);
	menuBar->Bind(wxEVT_MENU, &Frame::aboutClicked, this, wxID_ABOUT);

	return menuBar;
}

wxPanel *Frame::createChessboard(wxWindow *parent) {
	auto *chessboardPanel = new wxPanel(parent, wxID_ANY);
	chessboardPanel->SetBackgroundColour(resources.getColor("border"));

	grid = new ChessboardGrid();
	if (!grid->Create(std::bind(&Frame::getBitmap, this, std::placeholders::_1, std::placeholders::_2),
					  std::bind(&Frame::getColor, this, std::placeholders::_1, std::placeholders::_2),
					  chessboardPanel, wxID_ANY, wxPoint(CHESSBOARD_BORDER_H, CHESSBOARD_BORDER_V))) {
#ifdef DEBUG
		std::cerr << "Cannot create ChessboardGrid" << std::endl;
#endif
		return nullptr;
	}

	chessboardPanel->SetInitialSize(grid->GetSize() + wxSize(CHESSBOARD_BORDER_H * 2, CHESSBOARD_BORDER_V * 2));

	return chessboardPanel;
}

const wxBitmap &Frame::getBitmap(const std::string &key, const wxBitmap &def) {
	return resources.getBitmap(key, def);
}

const wxColour &Frame::getColor(const std::string &key, const wxColour &def) {
	return resources.getColor(key, def);
}


// Events

void Frame::onGameEvent(MatchManager::State state) {
	switch (state) {
		case MatchManager::TURN_PLAYER:
			SetStatusText(wxString::Format(_("%s | Difficulty: %d"), _("Your turn"), grid->getDifficulty()));
			break;
		case MatchManager::TURN_PC:
			SetStatusText(wxString::Format(_("%s | Difficulty: %d"), _("PC turn"), grid->getDifficulty()));
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
			wxMessageDialog(this, _("You can't make any moves with this piece"), _("Invalid selection")).ShowModal();
			break;
		case MatchManager::ILLEGAL_MOVE:
			wxMessageDialog(this, _("You can't move the selected piece is this position"), _("Invalid move")).ShowModal();
	}
}

void Frame::newMatchClicked(wxCommandEvent &) {
	if (grid->isPlaying()) {
		wxMessageDialog dialog(this, _("Are you sure you want to leave the game?"), _("New match"), wxYES_NO);
		if (dialog.ShowModal() != wxID_YES) return;
	}

	grid->newMatch(mGameDifficulty, mIsPcFirstPlayer);
}

void Frame::closeFrame(wxCommandEvent &) {
	Close();
}

void Frame::changeDifficultyClicked(wxCommandEvent &) {
	if (grid->isPlaying()) {
		wxMessageDialog dialog(this, _("Are you sure you want to leave the game?"), _("New match"), wxYES_NO);
		if (dialog.ShowModal() != wxID_YES) return;
	}

	while (true) {
		wxString message = wxString::Format(_("Choose game difficulty from %d to %d"), MatchManager::minGD,
		                                    MatchManager::maxGD);
		wxTextEntryDialog dialog(this, message, _("Game difficulty"));
		if (dialog.ShowModal() != wxID_OK) return;

		long value;
		if (dialog.GetValue().ToLong(&value) && value >= MatchManager::minGD && value <= MatchManager::maxGD &&
		    grid->newMatch(mGameDifficulty = value, mIsPcFirstPlayer)) {
			return;
		}
	}
}

void Frame::flipFirstPlayer(wxCommandEvent &) {
	if (grid->isPlaying()) {
		wxMessageDialog dialog(this, _("Are you sure you want to leave the game?"), _("New match"), wxYES_NO);
		if (dialog.ShowModal() != wxID_YES) return;
	}

	grid->newMatch(mGameDifficulty, mIsPcFirstPlayer = !mIsPcFirstPlayer);
}

void Frame::aboutClicked(wxCommandEvent &) {
	wxAboutDialogInfo dialog;
	dialog.SetName(wxFrame::GetTitle());
	dialog.SetIcon(programLogo);
	dialog.SetVersion(PROJECT_VERSION);
	dialog.SetDescription(PROJECT_DESCRIPTION);
	dialog.SetCopyright(PROJECT_COPYRIGHT);
	dialog.SetDevelopers(developers);
	dialog.SetLicense(PROJECT_LICENSE);

	wxAboutBox(dialog, this);
}
