#pragma once

#include <vector>
#include "chess.hpp"
#include "diagnostics.hpp"


class Engine {
private:
    // Constants
    const int PAWN_VALUE = 100;
    const int KNIGHT_VALUE = 275;
    const int BISHOP_VALUE = 300;
    const int ROOK_VALUE = 500;
    const int QUEEN_VALUE = 900;
    const float POSITIVE_INFINITY = 999999999.0;
    const float NEGATIVE_INFINITY = -999999999.0;
    float BEST_POSSIBLE_VALUE;
    float WORST_POSSIBLE_VALUE;
    int MAX_DEPTH;

    Diagnostics diagnostics;
    bool debug_mode = true;

    // Objects
    chess::Board* board;
    chess::Color color;

public:
    // PLACE HOLDER: FIXME
    std::vector<chess::Move> total_path;

    Engine(chess::Color color, int maxdepth);
    void setBoard(chess::Board* b);

    chess::Move think();
    float evaluate_fen();
    chess::Move search_begin();
    float quiescense_search(float alpha, float beta);
    float search(
        int depth,
        float alpha,
        float beta,
        int player
    );

    void order_moves(std::vector<chess::Move>& moves);
    int get_piece_value(chess::Piece p);
};
