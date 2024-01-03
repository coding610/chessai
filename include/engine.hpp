#pragma once

#include <vector>
#include "chess.hpp"


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

    int positions_searched = 0;
    int engine_move_index = 0;
    int ab = 0;
    bool ab_pruning;
    bool write_low_depth_search_log = false;

    // Objects
    chess::Board* board;
    chess::Color color;

public:
    std::vector<std::vector<chess::Move>> best_path;
    std::vector<chess::Move> total_path;

    Engine(chess::Color color, bool ab_pruning, int maxdepth);
    void setBoard(chess::Board* b);

    chess::Move think();
    float evaluate_fen();
    chess::Move search_begin();
    float quiescense_search(float alpha, float beta);
    float search(
        int depth,
        float alpha,
        float beta
    );

    std::vector<chess::Move> order_moves(std::vector<chess::Move> moves);
    int get_piece_value(chess::Piece p);
};
