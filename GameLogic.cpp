#include "GameLogic.h"

Chessboard::Move *GameLogic::calculateBestMove(Chessboard &board, int depth) {
    if (depth < 0) return nullptr;

    Chessboard::Move *res_move = nullptr;
    int score, bestScore = INT_MIN;
    int alpha = INT_MIN, beta = INT_MAX;

    std::vector<Chessboard::Move *> moves = board.findMoves(true);
    for (Chessboard::Move *move : moves) {
        score = minimax(move, move->m_score, false, depth, alpha, beta);
        if (score > bestScore) {
            bestScore = score;
            res_move = move;
        }
        if (score > alpha) {
            alpha = score;
            if (beta <= alpha)
                break; // ignore subtree
        }
    }

    if (moves.empty()) {
        return nullptr;
    } else if (res_move == nullptr) {
        // bestScore equals INT_MIN
        //res_move = moves.at(0); // all moves have the minimum m_score
        res_move = moves.front();
    }

    for (Chessboard::Move *move : moves) {
        if (move != res_move)
            delete move;
    }

    return res_move;
}

int GameLogic::minimax(Chessboard::Move *start_move, int eaten, bool maximizing, int depth, int alpha, int beta) {
    if (depth == 0) return eaten; // depth limit reached

    int bestScore, score;

    if (maximizing) {
        bestScore = INT_MIN;
        std::vector<Chessboard::Move *> moves = Chessboard::findMoves(start_move, true);
        for (Chessboard::Move *move : moves) {
            score = minimax(move, eaten + move->m_score, false, depth - 1, alpha, beta);
            if (score > bestScore) {
                bestScore = score;
            }
            if (score > alpha) {
                alpha = score;
                if (beta <= alpha)
                    break; // ignore subtree
            }
        }

        for (Chessboard::Move *move : moves) {
            delete move;
        }
        return bestScore;
    }

    bestScore = INT_MAX;
    std::vector<Chessboard::Move *> moves = Chessboard::findMoves(start_move, false);
    for (Chessboard::Move *move : moves) {
        score = minimax(move, eaten - move->m_score, true, depth - 1, alpha, beta);
        if (score < bestScore) {
            bestScore = score;
        }
        if (score < beta) {
            beta = score;
            if (beta <= alpha)
                break; // ignore subtree
        }
    }

    for (Chessboard::Move *move : moves) {
        delete move;
    }
    return bestScore;
}
