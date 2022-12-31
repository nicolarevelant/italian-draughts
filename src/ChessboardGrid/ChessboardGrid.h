#ifndef ITALIAN_DRAUGHTS_CHESSBOARD_GRID_H
#define ITALIAN_DRAUGHTS_CHESSBOARD_GRID_H

#include "../ChessboardSquare/ChessboardSquare.h"
#include "../ChessboardManager/ChessboardManager.h"
#include "ProjectVariables.h"
#include <wx/wx.h>
#include <array>

class ChessboardGrid : public wxPanel {
public:
	ChessboardGrid();
	~ChessboardGrid() override = default;

	ChessboardGrid(const wxColour &darkColor, const wxColour &lightColor,
	               const wxColour &focusColor, const wxColour &possibleMoveColor,
				   wxWindow *parent, wxWindowID winId = wxID_ANY,
				   const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxDefaultSize);

	bool Create(const wxColour &darkColor, const wxColour &lightColor,
	            const wxColour &focusColor, const wxColour &possibleMoveColor,
	            wxWindow *parent, wxWindowID winId = wxID_ANY,
				const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxDefaultSize);

	void updateBoardAndIcons(ChessboardManager::Move *move);

private:
	ChessboardGrid(const ChessboardGrid &); // prevents copy-constructor
	std::array<ChessboardSquare *, 64> chessboard{};
	ChessboardManager board;

	wxPen focusBorder, possibleMoveBorder;
	const wxBitmap pcPawn, pcDame, plPawn, plDame;

	void OnItemMouseClicked(wxMouseEvent &);
};


#endif //ITALIAN_DRAUGHTS_CHESSBOARD_GRID_H
