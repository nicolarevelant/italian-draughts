#ifndef ITALIAN_DRAUGHTS_FRAME_H
#define ITALIAN_DRAUGHTS_FRAME_H

#include "ChessboardSquare.h"
#include "MinimaxThread.h"
#include "wx/aboutdlg.h"

#define DEF_MIN_GD 1
#define DEF_MAX_GD 10

#define DEF_DARK_COLOR wxColour(32, 32, 32)
#define DEF_LIGHT_COLOR wxColour(140, 140, 140)

class Frame : public wxFrame {
public:
	static const int squareSize = DEF_SQUARE_SIZE, border = 40, padding = 20, selectedNone = -1, minGD = DEF_MIN_GD, maxGD = DEF_MAX_GD;

	explicit Frame(const std::string &locale = "", const std::string &theme = "");

	~Frame() override;

private:
	enum MenuItems {
		NEW_MATCH = 1,
		CHANGE_GD,
		THREAD_FINISH
	};
	const StringResources strings;
	const ColorResources colors;
	const wxPen focusBorder, possibleMoveBorder;
	const wxBitmap pcPawn, pcDame, plPawn, plDame;
	const wxArrayString developers = wxArrayString(1, {"Nicola Revelant <nicolarevelant44@gmail.com>"});

	ChessboardSquare *chessboard[CHESSBOARD_SIZE * CHESSBOARD_SIZE]{};
	int selectedPos = selectedNone, gameDifficult = minGD;
	bool m_isPlaying = false, m_isEnd = false, m_pcTurn = false;
	Chessboard board{};
	Chessboard::MoveList moves; // list of moves the player can do

	wxMenuBar *createMenuBar();

	wxPanel *createChessboard(wxWindow *parent);

	wxPanel *createChessboardGrid(wxWindow *parent,
	                              const wxPoint &pos = wxDefaultPosition,
	                              const wxSize &size = wxDefaultSize);

	void updateStatusText(const wxString &text = wxEmptyString);

	void updateBoardAndIcons(Chessboard::Move *move = nullptr);

	void updateChessboard(Chessboard::Move *move = nullptr);

	void checkUpdateSelection(int newSelection = selectedNone);

	Chessboard::Move *findPlayerMove(int oldIndex, int newIndex);

	void deleteMoves();

	// events
	void newMatchClicked(wxCommandEvent &);

	void closeFrame(wxCommandEvent &);

	void changeDifficultClicked(wxCommandEvent &);

	void aboutClicked(wxCommandEvent &);

	void OnItemMouseClicked(wxMouseEvent &);

	void onThreadFinished(wxCommandEvent &event);
};

#endif //ITALIAN_DRAUGHTS_FRAME_H
