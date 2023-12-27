#pragma once

#include <string>
#include "chess.hpp"


class Engine {
private:
    // Constants
    const int PAWN_VALUE = 100;
    const int KNIGHT_VALUE = 275;
    const int BISHOP_VALUE = 300;
    const int ROOK_VALUE = 500;
    const int QUEEN_VALUE = 900;
    int MAX_DEPTH; // NEED TO BE EVEN
    int positions_searched = 0;
    bool ab_pruning;
    int ab = 0;

    // Objects
    chess::Board* board;
    chess::Color color;
    float worst_value;
    float best_value;

public:
    std::vector<chess::Move> best_path;
    Engine(chess::Color color, bool ab_pruning, int depth);
    void setBoard(chess::Board* b);

    chess::Move think();
    float search(chess::Board* board, int depth, float alpha, float beta, bool clrw);
    float evaluate_fen(std::string fen);
};
