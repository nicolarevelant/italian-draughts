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

#ifndef FRAME_H
#define FRAME_H

#include "Resources/Resources.h"
#include "ChessboardGrid/ChessboardGrid.h"

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

private:
	Frame(const Frame &); // prevents copy-constructor

	enum MenuItems {
		NEW_MATCH = 1,
		CHANGE_GD,
		TOGGLE_FIRST_PLAYER,
	};

	Resources resources;
	const wxArrayString developers = wxArrayString(1, {"Nicola Revelant <nicolarevelant44@gmail.com>"});
	ChessboardGrid *grid;
	int mGameDifficulty;
	bool mIsPcFirstPlayer;

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
	void onGameEvent(MatchManager::State state);

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

#endif // FRAME_H
