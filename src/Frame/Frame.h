// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2023  Nicola Revelant

#ifndef ITALIAN_DRAUGHTS_FRAME_H
#define ITALIAN_DRAUGHTS_FRAME_H

#include "../Resources/Resources.h"
#include "MatchManager/MatchManager.h"
#include "../ChessboardGrid/ChessboardGrid.h"
#include "wx/wx.h"

#define CHESSBOARD_BORDER_H 40
#define CHESSBOARD_BORDER_V 40
#define CHESSBOARD_MARGIN_H 60
#define CHESSBOARD_MARGIN_V 40

class Frame : public wxFrame {
public:
	Frame();

	/**
	 * Creates a new Frame
	 * @param parent Parent
	 * @param theme Theme name (optional)
	 */
	explicit Frame(wxWindow *parent, const std::string &theme = "");

	bool Create(wxWindow *parent, const std::string &theme = "");

	~Frame() override;

private:
	Frame(const Frame &); // prevents copy-constructor

	enum MenuItems {
		NEW_MATCH = 1,
		CHANGE_GD,
		TOGGLE_FIRST_PLAYER,
	};

	Resources resources{DATA_PATH};
	const wxArrayString developers = wxArrayString(1, {"Nicola Revelant <nicolarevelant44@gmail.com>"});

	MatchManager *chessboardManager{};

	/**
	 * Creates a new menu bar and associates events
	 * @return The new menu bar
	 */
	wxMenuBar *createMenuBar();

	/**
	 * Creates a wxPanel that contains the ChessboardGrid
	 * @param parent Parent
	 * @return The new wxPanel
	 */
	wxPanel *createChessboard(wxWindow *parent);

	const wxBitmap &getBitmap(const std::string &key, const wxBitmap &def);

	const wxColour &getColor(const std::string &key, const wxColour &def);

	// events

	/**
	 * Update status bar
	 * @param updateType Update type
	 */
	void onGameEvent(enum MatchManager::UpdateType updateType);

	/**
	 * Starts a new match
	 */
	void newMatchClicked(wxCommandEvent &);

	/**
	 * Close frame
	 */
	void closeFrame(wxCommandEvent &);

	/**
	 * Change difficulty
	 */
	void changeDifficultyClicked(wxCommandEvent &);

	/**
	 * Change first player
	 */
	void flipFirstPlayer(wxCommandEvent &);

	/**
	 * Shows about dialog
	 */
	void aboutClicked(wxCommandEvent &);
};

#endif //ITALIAN_DRAUGHTS_FRAME_H
