//
// Created by nicola
//

#ifndef ITALIAN_DRAUGHTS_CHESSBOARD_H
#define ITALIAN_DRAUGHTS_CHESSBOARD_H

#include <bits/stdc++.h>

class Chessboard {
public:
    Chessboard();
    virtual ~Chessboard();

    enum PieceType {
        EMPTY = 0, // no piece
        PC_PAWN,   // white pawn
        PC_DAME,   // white dame
        PL_PAWN,   // black pawn
        PL_DAME    // black dame
    };

    struct Move {
        Move(const PieceType *mat, bool eatenFromPawn, int score)
        : m_mat(mat), m_eatenFromPawn(eatenFromPawn), m_score(score) {}

        ~Move() { delete[]m_mat; }

        const PieceType *m_mat;
        const bool m_eatenFromPawn;
        const int m_score;
    };
    typedef std::vector<Move *> MoveList;

    static PieceType *copyMatrix(const PieceType *from);
    [[nodiscard]] PieceType get(int index) const;
    void updateBoard(Move *move);
    [[nodiscard]] MoveList findMoves(bool pcTurn) const;
    static MoveList findMoves(Move *start_move, bool pcTurn);

private:
    PieceType m_mat[64]{};
    void setDefaultLayout();

    static bool addStepDownSx(MoveList &moves, const PieceType *mat, int s_row, int s_col, int score);
    static bool addStepDownDx(MoveList &moves, const PieceType *mat, int s_row, int s_col, int score);
    static bool addStepUpSx(MoveList &moves, const PieceType *mat, int s_row, int s_col, int score);
    static bool addStepUpDx(MoveList &moves, const PieceType *mat, int s_row, int s_col, int score);

    static MoveList findMoves(const PieceType *mat, bool pcTurn);

};

#endif //ITALIAN_DRAUGHTS_CHESSBOARD_H
