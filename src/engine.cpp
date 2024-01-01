#include <algorithm>
#include <string>
#include "chess.hpp"
#include "engine.hpp"
#include "utils.hpp"

using utils::DEB;
using utils::DBN;

chess::Move Engine::think() {

    chess::Move best_move;
    float best_evaluation = utils::is_clrw(this->color) ? this->NEGATIVE_INFINITY : this->POSITIVE_INFINITY;

    auto legal_moves = utils::legal_moves(*this->board);
    for (auto& move : legal_moves) {
        this->board->makeMove(move);

        float evaluation = this->search(
            *this->board,
            this->MAX_DEPTH,
            this->NEGATIVE_INFINITY,
            this->POSITIVE_INFINITY,
            utils::is_clrw(this->color)
        );

        this->board->unmakeMove(move);

        if (this->color == chess::Color::WHITE) {
            best_evaluation = std::max(evaluation, best_evaluation);
        } else {
            best_evaluation = std::min(evaluation, best_evaluation);
        } if (evaluation == best_evaluation) {
            best_move = move;
        }
    }

    return best_move;
}

float Engine::search(
    chess::Board& board,
    int depth,
    float alpha,
    float beta,
    bool maximizing_player
) {
    return 0.0;
}

float Engine::evaluate_fen(std::string fen) {
    positions_searched++;

    int score = 0;
    for (char& c : fen) {
        switch (c) {
            case ' ': return score;
            case 'r' : score -= this->ROOK_VALUE; break;
            case 'n' : score -= this->KNIGHT_VALUE; break;
            case 'b' : score -= this->BISHOP_VALUE; break;
            case 'q' : score -= this->QUEEN_VALUE; break;
            case 'p' : score -= this->PAWN_VALUE; break;
            case 'R' : score += this->ROOK_VALUE; break;
            case 'N' : score += this->KNIGHT_VALUE; break;
            case 'B' : score += this->BISHOP_VALUE; break;
            case 'Q' : score += this->QUEEN_VALUE; break;
            case 'P' : score += this->PAWN_VALUE; break;
        }
    }

    return 0.0;
}

Engine::Engine(chess::Color color, bool ab_pruning, int MAX_DEPTH) {
    this->MAX_DEPTH = MAX_DEPTH;
    this->ab_pruning = ab_pruning;

    this->color = color;
    if (color == chess::Color::WHITE) {
        this->BEST_POSSIBLE_VALUE = this->POSITIVE_INFINITY;
        this->WORST_POSSIBLE_VALUE = this->NEGATIVE_INFINITY;
    } else {
        this->BEST_POSSIBLE_VALUE = this->NEGATIVE_INFINITY;
        this->WORST_POSSIBLE_VALUE = this->POSITIVE_INFINITY;
    }
}

void Engine::setBoard(chess::Board* b) {
    this->board = b;
}
