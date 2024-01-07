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
    int MAX_DEPTH;

    Diagnostics diagnostics;
    bool debug_mode = true;

    // Objects
    chess::Board* board;
    chess::Color color;

public:
    // PLACE HOLDER: FIXME
    EngineV1(chess::Color clr, int maxdepth);
    void set_board(chess::Board* b) override;

    chess::Move think() override;
    chess::Move search_begin();
    float search(int depth, float alpha, float beta, int player);

    float quiescense_search(float alpha, float beta);

    void order_moves(std::vector<chess::Move>& moves);
    int see(chess::Square& square, int side);
    chess::Square smallest_attacker(chess::Square& square, int side);

    int evaluate_fen();
    int get_piece_value(chess::Piece p);
};
