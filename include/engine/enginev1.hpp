#pragma once

#include <vector>
#include "chess.hpp"
#include "engine/engine.hpp"
#include "diagnostics.hpp"


class EngineV1 : public Engine {
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
    bool debug_mode = false;

    // Objects
    chess::Board* board;
    chess::Color color;

public:
    // PLACE HOLDER: FIXME
    EngineV1(chess::Color clr, int maxdepth);
    void setBoard(chess::Board* b) override;

    chess::Move think() override;
    float search(int depth, float alpha, float beta, int player);
    chess::Move search_begin();
    float evaluate_fen();

    float quiescense_search(float alpha, float beta);
    void order_moves(std::vector<chess::Move>& moves);
    float see();
    int get_piece_value(chess::Piece p);
};
