#pragma once

#include <string>
#include <limits>
#include "chess.hpp"


class Engine {
private:
    // Constants
    const int PAWN_VALUE = 100;
    const int KNIGHT_VALUE = 275;
    const int BISHOP_VALUE = 300;
    const int ROOK_VALUE = 500;
    const int QUEEN_VALUE = 900;
    const float POSITIVE_INFINITY = std::numeric_limits<float>::max();;
    const float NEGATIVE_INFINITY = -std::numeric_limits<float>::max();
    float BEST_POSSIBLE_VALUE;
    float WORST_POSSIBLE_VALUE;
    int MAX_DEPTH;

    int positions_searched = 0;
    bool ab_pruning;
    int ab = 0;

    // Objects
    chess::Board* board;
    chess::Color color;

public:
    std::vector<std::vector<chess::Move>> best_path;
    std::vector<chess::Move> total_path;

    Engine(chess::Color color, bool ab_pruning, int depth);
    void setBoard(chess::Board* b);

    chess::Move think();
    float search(
        chess::Board& board,
        int depth,
        float alpha,
        float beta,
        bool maximizing_player
    );
    chess::Movelist order_moves(chess::Movelist moves);
    float evaluate_fen(std::string fen);
};
