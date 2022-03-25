#include "Chessboard.h"

Chessboard::Chessboard() = default;

Chessboard::~Chessboard() = default;

Chessboard::PieceType *Chessboard::copyMatrix(const PieceType *from) {
    auto to = new PieceType[64];
    memcpy(to, from, sizeof(PieceType) * 64);
    return to;
}

Chessboard::MoveList Chessboard::findMoves(Chessboard::Move *start_move, bool pcTurn) {
    return findMoves(start_move->m_mat, pcTurn);
}

Chessboard::MoveList Chessboard::findMoves(const PieceType *mat, bool pcTurn) {
    MoveList moves;

    if (pcTurn) {
        for (int row = 0, col; row < 8; row++) {
            for (col = 0; col < 8; col++) {
                switch (mat[row * 8 + col]) {
                    case PC_DAME:
                        addStepUpSx(moves, mat, row, col, 0);
                        addStepUpDx(moves, mat, row, col, 0);
                        addStepDownSx(moves, mat, row, col, 0);
                        addStepDownDx(moves, mat, row, col, 0);
                        break;
                    case PC_PAWN:
                        addStepDownSx(moves, mat, row, col, 0);
                        addStepDownDx(moves, mat, row, col, 0);
                        break;
                    case EMPTY:
                    case PL_PAWN:
                    case PL_DAME:
                        break;
                }
            }
        }
    } else {
        for (int row = 0, col; row < 8; row++) {
            for (col = 0; col < 8; col++) {
                switch (mat[row * 8 + col]) {
                    case PL_DAME:
                        addStepDownSx(moves, mat, row, col, 0);
                        addStepDownDx(moves, mat, row, col, 0);
                        addStepUpSx(moves, mat, row, col, 0);
                        addStepUpDx(moves, mat, row, col, 0);
                        break;
                    case PL_PAWN:
                        addStepUpSx(moves, mat, row, col, 0);
                        addStepUpDx(moves, mat, row, col, 0);
                        break;
                    case EMPTY:
                    case PC_PAWN:
                    case PC_DAME:
                        break;
                }
            }
        }
    }

    // if there is a move with m_score > 0 then it deletes the non-eating moves
    for (Move *move : moves) {
        // if a pawn can eat something, moves that eat nothing are removed from the list
        if (move->m_eatenFromPawn) {
            MoveList new_moves;
            for (Move *int_move : moves) {
                if (int_move->m_score > 0) {
                    new_moves.push_back(int_move);
                } else {
                    delete int_move;
                }
            }
            return new_moves;
        }
    }

    return moves;
}

bool Chessboard::addStepDownSx(MoveList &moves, const PieceType *mat, int s_row, int s_col, int score) {
    if (s_row == 7 || s_col == 0)
        return false;

    int row = s_row + 1, col = s_col - 1;
    PieceType s_value = mat[s_row * 8 + s_col];
    PieceType *copy;
    bool isValid = true;

    int mid_value = mat[row * 8 + col];
    if (mid_value == EMPTY) {
        // move without jump
        if (score == 0) {
            copy = Chessboard::copyMatrix(mat);
            copy[s_row * 8 + s_col] = EMPTY;
            copy[row * 8 + col] = row == 7 && s_value == PC_PAWN ? PC_DAME : s_value;
            moves.push_back(new Move(copy, false, 0));
            return true;
        }
        return false;
    }

    if (row == 7 || col == 0) {
        return false;
    }

    if (s_value == PC_PAWN) {
        if (mid_value != PL_PAWN)
            return false; // white man only eat black man
        row++;
        col--;
        if (mat[row * 8 + col] != EMPTY) {
            return false;
        }

        // possible jump, check only downSx and downDx
        copy = Chessboard::copyMatrix(mat);
        copy[s_row * 8 + s_col] = EMPTY;
        copy[row * 8 + col - 7] = EMPTY;
        copy[row * 8 + col] = row == 7 ? PC_DAME : PC_PAWN;
        score++;

        if (addStepDownSx(moves, copy, row, col, score))
            isValid = false;

        if (addStepDownDx(moves, copy, row, col, score))
            isValid = false;

        if (isValid)
            moves.push_back(new Move(copy, true, score));
        else
            delete[]copy;

        return true;
    }
    if (s_value == PC_DAME) {
        if (mid_value == PC_DAME || mid_value == PC_PAWN)
            return false;
    }
    if (s_value == PL_DAME) {
        if (mid_value == PL_DAME || mid_value == PL_PAWN)
            return false;
    }

    row++;
    col--;
    if (mat[row * 8 + col] != EMPTY) {
        return false;
    }

    // possible jump
    copy = Chessboard::copyMatrix(mat);
    copy[s_row * 8 + s_col] = EMPTY;
    copy[row * 8 + col - 7] = EMPTY;
    copy[row * 8 + col] = s_value;
    score += 2;

    if (addStepDownSx(moves, copy, row, col, score))
        isValid = false;

    if (addStepDownDx(moves, copy, row, col, score))
        isValid = false;

    if (addStepUpSx(moves, copy, row, col, score))
        isValid = false;

    if (addStepUpDx(moves, copy, row, col, score))
        isValid = false;

    if (isValid)
        moves.push_back(new Move(copy, false, score));
    else
        delete[]copy;

    return true;
}

bool Chessboard::addStepDownDx(MoveList &moves, const PieceType *mat, int s_row, int s_col, int score) {
    if (s_row == 7 || s_col == 7)
        return false;
    int row = s_row + 1, col = s_col + 1;
    PieceType s_value = mat[s_row * 8 + s_col];
    PieceType *copy;
    bool isValid = true;

    int mid_value = mat[row * 8 + col];
    if (mid_value == EMPTY) {
        if (score == 0) {
            copy = Chessboard::copyMatrix(mat);
            copy[s_row * 8 + s_col] = EMPTY;
            copy[row * 8 + col] = row == 7 && s_value == PC_PAWN ? PC_DAME : s_value;
            moves.push_back(new Move(copy, false, 0));
            return true;
        }
        return false;
    }

    if (row == 7 || col == 7) {
        return false;
    }

    if (s_value == PC_PAWN) {
        if (mid_value != PL_PAWN)
            return false;
        row++;
        col++;
        if (mat[row * 8 + col] != EMPTY) {
            return false;
        }

        copy = Chessboard::copyMatrix(mat);
        copy[s_row * 8 + s_col] = EMPTY;
        copy[row * 8 + col - 9] = EMPTY;
        copy[row * 8 + col] = row == 7 ? PC_DAME : PC_PAWN;
        score++;

        if (addStepDownSx(moves, copy, row, col, score))
            isValid = false;

        if (addStepDownDx(moves, copy, row, col, score))
            isValid = false;

        if (isValid)
            moves.push_back(new Move(copy, true, score));
        else
            delete[]copy;

        return true;
    }
    if (s_value == PC_DAME) {
        if (mid_value == PC_DAME || mid_value == PC_PAWN)
            return false;
    }
    if (s_value == PL_DAME) {
        if (mid_value == PL_DAME || mid_value == PL_PAWN)
            return false;
    }

    row++;
    col++;
    if (mat[row * 8 + col] != EMPTY) {
        return false;
    }

    copy = Chessboard::copyMatrix(mat);
    copy[s_row * 8 + s_col] = EMPTY;
    copy[row * 8 + col - 9] = EMPTY;
    copy[row * 8 + col] = s_value;
    score += 2;

    if (addStepDownSx(moves, copy, row, col, score))
        isValid = false;

    if (addStepDownDx(moves, copy, row, col, score))
        isValid = false;

    if (addStepUpSx(moves, copy, row, col, score))
        isValid = false;

    if (addStepUpDx(moves, copy, row, col, score))
        isValid = false;

    if (isValid)
        moves.push_back(new Move(copy, false, score));
    else
        delete[]copy;

    return true;
}

bool Chessboard::addStepUpSx(MoveList &moves, const PieceType *mat, int s_row, int s_col, int score) {
    if (s_row == 0 || s_col == 0)
        return false;
    int row = s_row - 1, col = s_col - 1;
    PieceType s_value = mat[s_row * 8 + s_col];
    PieceType *copy;
    bool isValid = true;

    int mid_value = mat[row * 8 + col];
    if (mid_value == EMPTY) {
        if (score == 0) {
            copy = Chessboard::copyMatrix(mat);
            copy[s_row * 8 + s_col] = EMPTY;
            copy[row * 8 + col] = row == 0 && s_value == PL_PAWN ? PL_DAME : s_value;
            moves.push_back(new Move(copy, false, 0));
            return true;
        }
        return false;
    }

    if (row == 0 || col == 0) {
        return false;
    }

    if (s_value == PL_PAWN) {
        if (mid_value != PC_PAWN)
            return false;
        row--;
        col--;
        if (mat[row * 8 + col] != EMPTY) {
            return false;
        }

        copy = Chessboard::copyMatrix(mat);
        copy[s_row * 8 + s_col] = EMPTY;
        copy[row * 8 + col + 9] = EMPTY;
        copy[row * 8 + col] = row == 0 ? PL_DAME : PL_PAWN;
        score++;

        if (addStepUpSx(moves, copy, row, col, score))
            isValid = false;

        if (addStepUpDx(moves, copy, row, col, score))
            isValid = false;

        if (isValid)
            moves.push_back(new Move(copy, true, score));
        else
            delete[]copy;

        return true;
    }
    if (s_value == PC_DAME) {
        if (mid_value == PC_DAME || mid_value == PC_PAWN)
            return false;
    }
    if (s_value == PL_DAME) {
        if (mid_value == PL_DAME || mid_value == PL_PAWN)
            return false;
    }

    row--;
    col--;
    if (mat[row * 8 + col] != EMPTY) {
        return false;
    }

    copy = Chessboard::copyMatrix(mat);
    copy[s_row * 8 + s_col] = EMPTY;
    copy[row * 8 + col + 9] = EMPTY;
    copy[row * 8 + col] = s_value;
    score += 2;

    if (addStepUpSx(moves, copy, row, col, score))
        isValid = false;

    if (addStepUpDx(moves, copy, row, col, score))
        isValid = false;

    if (addStepUpSx(moves, copy, row, col, score))
        isValid = false;

    if (addStepUpDx(moves, copy, row, col, score))
        isValid = false;

    if (isValid)
        moves.push_back(new Move(copy, false, score));
    else
        delete[]copy;

    return true;
}

bool Chessboard::addStepUpDx(MoveList &moves, const PieceType *mat, int s_row, int s_col, int score) {
    if (s_row == 0 || s_col == 7)
        return false;
    int row = s_row - 1, col = s_col + 1;
    PieceType s_value = mat[s_row * 8 + s_col];
    PieceType *copy;
    bool isValid = true;

    int mid_value = mat[row * 8 + col];
    if (mid_value == EMPTY) {
        if (score == 0) {
            copy = Chessboard::copyMatrix(mat);
            copy[s_row * 8 + s_col] = EMPTY;
            copy[row * 8 + col] = row == 0 && s_value == PL_PAWN ? PL_DAME : s_value;
            moves.push_back(new Move(copy, false, 0));
            return true;
        }
        return false;
    }

    if (row == 0 || col == 7) {
        return false;
    }

    if (s_value == PL_PAWN) {
        if (mid_value != PC_PAWN)
            return false;
        row--;
        col++;
        if (mat[row * 8 + col] != EMPTY) {
            return false;
        }

        copy = Chessboard::copyMatrix(mat);
        copy[s_row * 8 + s_col] = EMPTY;
        copy[row * 8 + col + 7] = EMPTY;
        copy[row * 8 + col] = row == 0 ? PL_DAME : PL_PAWN;
        score++;

        if (addStepUpSx(moves, copy, row, col, score))
            isValid = false;

        if (addStepUpDx(moves, copy, row, col, score))
            isValid = false;

        if (isValid)
            moves.push_back(new Move(copy, true, score));
        else
            delete[]copy;

        return true;
    }
    if (s_value == PC_DAME) {
        if (mid_value == PC_DAME || mid_value == PC_PAWN)
            return false;
    }
    if (s_value == PL_DAME) {
        if (mid_value == PL_DAME || mid_value == PL_PAWN)
            return false;
    }

    row--;
    col++;
    if (mat[row * 8 + col] != EMPTY) {
        return false;
    }

    copy = Chessboard::copyMatrix(mat);
    copy[s_row * 8 + s_col] = EMPTY;
    copy[row * 8 + col + 7] = EMPTY;
    copy[row * 8 + col] = s_value;
    score += 2;

    if (addStepDownSx(moves, copy, row, col, score))
        isValid = false;

    if (addStepDownDx(moves, copy, row, col, score))
        isValid = false;

    if (addStepUpSx(moves, copy, row, col, score))
        isValid = false;

    if (addStepUpDx(moves, copy, row, col, score))
        isValid = false;

    if (isValid)
        moves.push_back(new Move(copy, false, score));
    else
        delete[]copy;

    return true;
}

Chessboard::MoveList Chessboard::findMoves(bool pcTurn) const {
    return Chessboard::findMoves(m_mat, pcTurn);
}

void Chessboard::updateBoard(Move *move) {
    if (move == nullptr) {
        setDefaultLayout();
    } else {
        for (int i = 0; i < 64; i++) {
            m_mat[i] = move->m_mat[i];
        }
    }
}

/**
 * Resets the initial pieces disposition
 */
void Chessboard::setDefaultLayout() {
    for (int i = 0; i < 64; i++) {
        if ((i / 8) % 2 == i % 2) {
            if (i < 24) {
                m_mat[i] = PC_PAWN;
            } else if (i >= 40) {
                m_mat[i] = PL_PAWN;
            } else {
                m_mat[i] = EMPTY;
            }
        } else {
            m_mat[i] = EMPTY;
        }
    }
}

Chessboard::PieceType Chessboard::get(int index) const {
    return m_mat[index];
}
