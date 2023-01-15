// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2023  Nicola Revelant

#ifndef ITALIAN_DRAUGHTS_FRAME_H
#define ITALIAN_DRAUGHTS_FRAME_H

#include "../Resources/Resources.h"
#include "MatchManager/MatchManager.h"
#include "../ChessboardGrid/ChessboardGrid.h"
#include "wx/wx.h"

#define DEF_DARK_COLOR wxColour(32, 32, 32)
#define DEF_LIGHT_COLOR wxColour(140, 140, 140)

#define CHESSBOARD_BORDER_H 40
#define CHESSBOARD_BORDER_V 40
#define CHESSBOARD_MARGIN_H 60
#define CHESSBOARD_MARGIN_V 40

class Frame : public wxFrame {
public:
	Frame();

	explicit Frame(wxWindow *parent, const std::string &theme = "");

	bool Create(wxWindow *parent, const std::string &theme = "");

	~Frame() override;

private:
	Frame(const Frame &); // prevents copy-constructor

	enum MenuItems {
		NEW_MATCH = 1,
		CHANGE_GD
	};

	Resources resources;
	const wxArrayString developers = wxArrayString(1, {"Nicola Revelant <nicolarevelant44@gmail.com>"});

	MatchManager *chessboardManager{};

	wxMenuBar *createMenuBar();

	wxPanel *createChessboard(wxWindow *parent);

	// events

	void onGameEvent(enum MatchManager::UpdateType updateType);

	void newMatchClicked(wxCommandEvent &);

	void closeFrame(wxCommandEvent &);

	void changeDifficultyClicked(wxCommandEvent &);

	void aboutClicked(wxCommandEvent &);
};

#endif //ITALIAN_DRAUGHTS_FRAME_H
