#ifndef ITALIAN_DRAUGHTS_CHESSBOARD_GRID_H
#define ITALIAN_DRAUGHTS_CHESSBOARD_GRID_H

#include "ChessboardSquare/ChessboardSquare.h"
#include "utils.h"
#include <wx/wx.h>

#define DEF_SQUARE_SIZE 74

/**
 * The mission of this class is to provide a wxPanel composed by a 8x8 grid
 */
class ChessboardGrid : public wxPanel {
public:
	ChessboardGrid();
	~ChessboardGrid() override;

	ChessboardGrid(const wxColour &darkColor, const wxColour &lightColor,
				   wxWindow *parent, wxWindowID winId = wxID_ANY,
				   const wxPoint &pos = wxDefaultPosition, int squareSize = DEF_SQUARE_SIZE);

	bool Create(const wxColour &darkColor, const wxColour &lightColor,
	            wxWindow *parent, wxWindowID winId = wxID_ANY,
				const wxPoint &pos = wxDefaultPosition, int squareSize = DEF_SQUARE_SIZE);

	/**
	 * Updates the disposition of the pieces in the chessboard and clears every border
	 * @param newDisposition The new disposition to apply
	 */
	void updateDisposition(Disposition &newDisposition);

	/**
	 * Set a new border for the square located at an specifica location
	 * @param index Position of the square
	 * @param border The new border
	 */
	void setBorder(int index, wxPen &border);

private:
	ChessboardGrid(const ChessboardGrid &); // prevents copy-constructor
	std::array<ChessboardSquare *, 64> chessboard{};

	wxBitmap pcPawn, pcDame, plPawn, plDame;
	MoveList moves; // list of moves the player can do

	int squareSize{};
};


#endif //ITALIAN_DRAUGHTS_CHESSBOARD_GRID_H
