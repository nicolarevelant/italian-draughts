#ifndef ITALIAN_DRAUGHTS_MYFRAME_H
#define ITALIAN_DRAUGHTS_MYFRAME_H

#include "ChessboardSquare.h"
#include "MinimaxThread.h"
#include "wx/aboutdlg.h"

#define DEF_DARK_COLOR wxColour(32, 32, 32)
#define DEF_LIGHT_COLOR wxColour(140, 140, 140)

class MyFrame : public wxFrame {
public:
    static const int squareSize = 74, border = 40, padding = 20, selectedNone = -1, minGD = 1, maxGD = 10;
    explicit MyFrame(const std::string &locale = "", const std::string &theme = "");
    ~MyFrame() override;

private:
    const StringResources strings;
    const ColorResources colors;
    const wxPen focusBorder, hoverBorder;
    const wxBitmap pcPawn, pcDame, plPawn, plDame;
    const wxArrayString developers = wxArrayString(1, {"Nicola Revelant <nicolarevelant44@gmail.com>"});

    ChessboardSquare *chessboard[64]{};
    int selectedPos = selectedNone, gameDifficult = minGD;
    bool m_isPlaying = false, m_isEnd = false, m_pcTurn = false;
    Chessboard board{};
    Chessboard::MoveList moves; // list of moves the player can do

    wxMenuBar *createMenuBar();

    void newMatchClicked(wxCommandEvent &);
    void closeFrame(wxCommandEvent &);
    void changeDifficultClicked(wxCommandEvent &);
    void aboutClicked(wxCommandEvent &);

    wxPanel *createChessboard(wxWindow *parent);
    wxPanel *createChessboardGrid(wxWindow *parent,
                                  const wxPoint& pos = wxDefaultPosition,
                                  const wxSize& size = wxDefaultSize);

    void updateStatusText(const wxString& text = wxEmptyString);
    void updateBoardAndIcons(Chessboard::Move *move = nullptr);

    // events
    void OnItemMouseEntered(wxMouseEvent &);
    void OnItemMouseExited(wxMouseEvent &);
    void OnItemMouseClicked(wxMouseEvent &);
    void onThreadFinished(wxCommandEvent &event);

    Chessboard::Move *findPlayerMove(int oldIndex, int newIndex);

    void deleteMoves();
};

#endif //ITALIAN_DRAUGHTS_MYFRAME_H
