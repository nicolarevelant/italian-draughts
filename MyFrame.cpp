#include "MyFrame.h"

enum MenuItems {
    NEW_MATCH = 1,
    CHANGE_GD,
    THREAD_FINISH
};

MyFrame::MyFrame(const std::string &locale, const std::string &theme)
: wxFrame(nullptr, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(
        squareSize * 8 + border * 2 + padding * 2 + 160,
        squareSize * 8 + border * 2 + padding * 2 + 51)),
        strings(locale), colors(theme),

        // colors and borders
        focusBorder(colors.get("focus-border", *wxWHITE), 3),
        hoverBorder(colors.get("hover-border", wxTransparentColour), 3),
        possibleMoveBorder(colors.get("possible-move-border", *wxRED), 3),

        // bitmaps
        pcPawn(SYSTEM_CFG_PATH"images/pcPawn.png"), pcDame(SYSTEM_CFG_PATH"images/pcDame.png"),
        plPawn(SYSTEM_CFG_PATH"images/plPawn.png"), plDame(SYSTEM_CFG_PATH"images/plDame.png") {
    wxFrame::SetTitle(strings.get("app.title", PROJECT_NAME));
    wxColour bgColor = colors.get("bg");

    wxFrame::SetMenuBar(createMenuBar());
    wxStatusBar *statusBar = wxFrame::CreateStatusBar();
    statusBar->SetBackgroundColour(bgColor);
    statusBar->SetForegroundColour(*wxWHITE);
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

    updateChessboard();

    Bind(wxEVT_MENU, &MyFrame::onThreadFinished, this, THREAD_FINISH);
}

wxMenuBar *MyFrame::createMenuBar() {
    auto *menuFile = new wxMenu;
    menuFile->Append(NEW_MATCH, strings["app.menu.new_match"]);
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT, strings["app.exit"], strings["app.exit.help"]);

    auto *menuSettings = new wxMenu;
    menuSettings->Append(CHANGE_GD, strings["app.menu.changeGD"],
                         strings["app.menu.changeGD.help"]);

    auto *menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT, strings["app.about"], strings["app.about.help"]);

    auto *menuBar = new wxMenuBar;
    menuBar->Append(menuFile, strings["app.menu.file"]);
    menuBar->Append(menuSettings, strings["app.menu.settings"]);
    menuBar->Append(menuHelp, strings["app.menu.help"]);

    menuBar->Bind(wxEVT_MENU, &MyFrame::newMatchClicked, this, NEW_MATCH);
    menuBar->Bind(wxEVT_MENU, &MyFrame::closeFrame, this, wxID_EXIT);
    menuBar->Bind(wxEVT_MENU, &MyFrame::changeDifficultClicked, this, CHANGE_GD);
    menuBar->Bind(wxEVT_MENU, &MyFrame::aboutClicked, this, wxID_ABOUT);

    return menuBar;
}

wxPanel *MyFrame::createChessboard(wxWindow *parent) {
    int gridSize = squareSize * 8;
    auto *chessboardPanel = new wxPanel(parent, wxID_ANY, wxDefaultPosition,
                                        wxSize(gridSize + border * 2, gridSize + border * 2));
    chessboardPanel->SetBackgroundColour(colors["border"]);
    createChessboardGrid(chessboardPanel, wxPoint(border, border),
                         wxSize(gridSize, gridSize));

    return chessboardPanel;
}

wxPanel *MyFrame::createChessboardGrid(wxWindow *parent, const wxPoint &pos, const wxSize &size) {
    auto *gridSizer = new wxGridSizer(8, 8, 0, 0);
    auto *chessboardGrid = new wxPanel(parent, wxID_ANY, pos, size);
    chessboardGrid->SetSizer(gridSizer);

    const wxColour lightColor = colors.get("light", DEF_LIGHT_COLOR);
    const wxColour darkColor = colors.get("dark", DEF_DARK_COLOR);
    chessboardGrid->SetBackgroundColour(lightColor);

    ChessboardSquare *square;
    for (int row = 0, i = 0, col; row < 8; row++) {
        for (col = 0; col < 8; col++, i++) {
            square = new ChessboardSquare(chessboardGrid, i);
            if (row % 2 == col % 2) {
                square->Bind(wxEVT_ENTER_WINDOW, &MyFrame::OnItemMouseEntered, this);
                square->Bind(wxEVT_LEAVE_WINDOW, &MyFrame::OnItemMouseExited, this);
                square->SetBackgroundColour(darkColor);
            } else {
                square->SetBackgroundColour(lightColor);
            }
            square->Bind(wxEVT_LEFT_UP, &MyFrame::OnItemMouseClicked, this);
            gridSizer->Add(square);
            chessboard[i] = square;
        }
    }

    chessboardGrid->Layout();
    return chessboardGrid;
}

//                             --- Menu events

void MyFrame::newMatchClicked(wxCommandEvent &) {
    if (m_pcTurn) return;

    if (m_isPlaying) {
        wxMessageDialog dialog(this, strings["game.leave"], strings["game.new"], wxYES_NO);
        if (dialog.ShowModal() != wxID_YES) return;
        m_isPlaying = false;
    }

    m_isEnd = false;

    updateChessboard();
    updateStatusText();

    Refresh();
}

void MyFrame::closeFrame(wxCommandEvent &) {
    Close();
}

void MyFrame::changeDifficultClicked(wxCommandEvent &) {
    if (m_pcTurn) {
        wxMessageDialog dialog(this, strings["game.wait.text"], strings["game.wait"]);
        dialog.ShowModal();
        return;
    }

    if (m_isPlaying) {
        wxMessageDialog dialog(this, strings["game.leave"], strings["game.new"], wxYES_NO);
        if (dialog.ShowModal() != wxID_YES) return;
    }

    while (true) {
        wxTextEntryDialog dialog(this, strings["game.diff.text"], strings["game.diff"]);
        if (dialog.ShowModal() != wxID_OK) return;
        auto strValue = dialog.GetValue();
        long value;
        if (strValue.ToLong(&value) && value >= minGD && value <= maxGD) {
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

void MyFrame::aboutClicked(wxCommandEvent &) {
    wxAboutDialogInfo dialog;
    dialog.SetName(strings.get("app.title", PROJECT_NAME));
    dialog.SetVersion(PROJECT_VERSION);
    dialog.SetDescription(strings["app.about.desc"]);
    dialog.SetCopyright(strings["app.about.copy"]);
    dialog.SetDevelopers(developers);
    wxAboutBox(dialog, this);
}

//                             --- End menu events

void MyFrame::updateStatusText(const wxString &text) {
    SetStatusText(wxString::Format(strings["app.statusbar.ph"],
                                   text.empty() ? strings[m_pcTurn ? "game.turn.pc" : "game.turn.you"] : text,
                                   gameDifficult));
}

void MyFrame::updateBoardAndIcons(Chessboard::Move *move) {
    board.updateBoard(move);
    for (int i = 0; i < 64; i++) {
        // update icons
        switch (board.get(i)) {
            case Chessboard::EMPTY:
                chessboard[i]->SetBitmap(wxNullBitmap);
                break;
            case Chessboard::PC_PAWN:
                chessboard[i]->SetBitmap(pcPawn);
                break;
            case Chessboard::PC_DAME:
                chessboard[i]->SetBitmap(pcDame);
                break;
            case Chessboard::PL_PAWN:
                chessboard[i]->SetBitmap(plPawn);
                break;
            case Chessboard::PL_DAME:
                chessboard[i]->SetBitmap(plDame);
                break;
        }
    }

    Refresh();
}

void MyFrame::updateChessboard(Chessboard::Move *move) {
    updateBoardAndIcons(move);
    delete move;

    deleteMoves();
    moves = board.findMoves(false);
}

void MyFrame::updateSelection(int newSelection) {
    for (auto &i : chessboard) {
        i->SetBorder(wxNullPen);
    }
    chessboard[newSelection]->SetBorder(focusBorder);

    // highlight the possible moves
    Chessboard::PieceType oldValue;
    for (Chessboard::Move *move : moves) {
        oldValue = move->m_mat[newSelection];
        if (oldValue != Chessboard::EMPTY) {
            continue; // this Chessboard::Move doesn't move the current selected piece
        }

        // highlight the unique empty checker in the current disposition that is filled in 'move'
        for (int i = 0; i < 64; i++) {
            if (board.get(i) == Chessboard::EMPTY && move->m_mat[i] != Chessboard::EMPTY) {
                chessboard[i]->SetBorder(possibleMoveBorder);
            }
        }
    }

    selectedPos = newSelection;
}

void MyFrame::OnItemMouseEntered(wxMouseEvent &event) {
    if (m_isEnd) return;
    int square_id = event.GetId();
    if (selectedPos == square_id) return;

    Chessboard::PieceType value = board.get(square_id);
    if (selectedPos == selectedNone) {
        // if not selected --> only if player piece
        if (value == Chessboard::PL_PAWN || value == Chessboard::PL_DAME) {
            chessboard[square_id]->SetBorder(hoverBorder);
            Refresh();
        }
    } else if (value == Chessboard::EMPTY) {
        // else (if selected) --> only if empty
        chessboard[square_id]->SetBorder(hoverBorder);
        Refresh();
    }
}

void MyFrame::OnItemMouseExited(wxMouseEvent &event) {
    int square_id = event.GetId();
    if (selectedPos == square_id) return;
    chessboard[square_id]->SetBorder(wxNullPen);
    Refresh();
}

void MyFrame::OnItemMouseClicked(wxMouseEvent &event) {
    if (m_isEnd) return;
    if (m_pcTurn) {
        wxMessageDialog dialog(this, strings["game.wait.text"], strings["game.wait"]);
        dialog.ShowModal();
        return;
    }
    int x, y;
    event.GetPosition(&x, &y);
    if (x < 0 || y < 0 || x >= squareSize || y >= squareSize) return; // illegal position

    int currentPos = event.GetId();

    if (selectedPos == selectedNone) {
        Chessboard::PieceType value = board.get(currentPos);
        if (value == Chessboard::PL_PAWN || value == Chessboard::PL_DAME) {
            updateSelection(currentPos);
            Refresh();
        }
        return;
    }

    // already selected

    // same square or white square, illegal move
    if (currentPos == selectedPos || (currentPos / 8) % 2 != currentPos % 2) {
        updateSelection(); // it removes also the possible move borders
        Refresh();
        return; // no move
    }

    Chessboard::PieceType value = board.get(currentPos);
    if (value != Chessboard::EMPTY) {
        if (value == Chessboard::PL_PAWN || value == Chessboard::PL_DAME) {
            // change selected position
            updateSelection(currentPos);
        } else {
            updateSelection();
        }
        Refresh();
        return; // no move
    }

    int oldSelection = selectedPos;
    updateSelection();
    Chessboard::Move *playerMove = findPlayerMove(oldSelection, currentPos);
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
        std::cout << "Thread error\n";
        exit(-1);
    }

    // The player move is in the moves vector that will be deleted after the PC move
    Refresh();
}

void MyFrame::onThreadFinished(wxCommandEvent &event) {
    m_pcTurn = false;
    auto *pcMove = reinterpret_cast<Chessboard::Move *>(event.GetClientData());
    if (pcMove == nullptr) {
        m_isEnd = true;
        m_isPlaying = false;
        updateStatusText(strings["game.you_won"]);
        wxMessageDialog dialog(this, strings["game.you_won"],
                               strings["game.over"]);
        dialog.ShowModal();
        return;
    }

    updateChessboard(pcMove);

    if (moves.empty()) {
        m_isEnd = true;
        m_isPlaying = false;
        updateStatusText(strings["game.you_lost"]);
        wxMessageDialog dialog(this, strings["game.you_lost"],
                               strings["game.over"]);
        dialog.ShowModal();
    } else {
        updateStatusText();
    }
}

Chessboard::Move *MyFrame::findPlayerMove(int oldIndex, int newIndex) {
    Chessboard::PieceType oldValue, newValue;
    for (Chessboard::Move *move: moves) {
        oldValue = move->m_mat[oldIndex];
        if (oldValue != Chessboard::EMPTY)
            continue; // this is not the correct move

        // here only if this move change the old position that becomes empty
        newValue = move->m_mat[newIndex];
        if (newValue == Chessboard::PL_PAWN || newValue == Chessboard::PL_DAME)
            return move; // this is the correct move from oldIndex to newIndex
    }

    return nullptr;
}

void MyFrame::deleteMoves() {
    for (Chessboard::Move *move: moves) {
        delete move;
    }
}

MyFrame::~MyFrame() {
    deleteMoves();
}
