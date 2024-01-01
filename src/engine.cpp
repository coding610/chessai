#include <algorithm>
#include <chrono>
#include <string>
#include <iostream>
#include "chess.hpp"
#include "engine.hpp"
#include "utils.hpp"

using utils::DEB;
using utils::DBN;

chess::Move Engine::think() {
    // this->positions_searched = 0;
    //
    // auto time_begin = std::chrono::steady_clock::now();
    // auto best_move = this->search_begin();
    // auto time_end = std::chrono::steady_clock::now();
    //
    // std::cout << "Positions searched: " << this->positions_searched << "\n";
    // std::cout << "Duration: "
    //     << std::chrono::duration_cast<std::chrono::milliseconds>(time_end - time_begin).count()
    //     << "ms\n";

    auto legal_moves = this->board->generate_legal_moves();
    return legal_moves[0];
}

/*
chess::Move Engine::search_begin() {
    chess::Move best_move;
    float best_evaluation = utils::is_clrw(this->color) ? this->NEGATIVE_INFINITY : this->POSITIVE_INFINITY;

    for (auto& move : utils::legal_moves(*this->board)) {
        this->board->makeMove(move);
        float evaluation = this->search(
            *this->board,
            this->MAX_DEPTH - 1,
            this->NEGATIVE_INFINITY,
            this->POSITIVE_INFINITY,
            utils::is_clrw(this->board->sideToMove())
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
    chess::Board& b,
    int depth,
    float alpha,
    float beta,
    bool maximizing_player // true if w, false if b
) {
    if (maximizing_player) {
        BEST_POSSIBLE_VALUE = this->POSITIVE_INFINITY;
        WORST_POSSIBLE_VALUE = this->NEGATIVE_INFINITY;
    } else {
        BEST_POSSIBLE_VALUE = this->NEGATIVE_INFINITY;
        WORST_POSSIBLE_VALUE = this->POSITIVE_INFINITY;
    }

    ////// HANDLE EDGE CASES //////
    if (depth == 0) {
        return this->evaluate_fen(b.getFen());
    } else if (this->board->isGameOver().second == chess::GameResult::WIN) {
        return BEST_POSSIBLE_VALUE;
    } else if (this->board->isGameOver().second == chess::GameResult::LOSE) {
        return WORST_POSSIBLE_VALUE;
    } else if (this->board->isGameOver().second == chess::GameResult::DRAW) {
        return 0.0;
    }

    ////// RECURSION //////
    float best_evaluation = WORST_POSSIBLE_VALUE;
    for (auto& move : utils::legal_moves(b)) {
        b.makeMove(move);
        float evaluation = this->search(
            b,
            depth - 1,
            alpha,
            beta,
            !maximizing_player
        );
        b.unmakeMove(move);

        ////// EVALUATION //////
        if (maximizing_player) {
            best_evaluation = std::max(evaluation, best_evaluation);
            alpha = std::max(alpha, evaluation);
        } else {
            best_evaluation = std::min(evaluation, best_evaluation);
            alpha = std::min(alpha, evaluation);
        }

        if (this->ab_pruning) {
            if (beta <= alpha) {
                break;
            }
        }
    }

    return best_evaluation;
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
*/

Engine::Engine(chess::Color color, bool ab_pruning, int MAX_DEPTH) {
    this->MAX_DEPTH = MAX_DEPTH;
    this->ab_pruning = ab_pruning;

    this->color = color;
}

void Engine::setBoard(chess::Board* b) {
    this->board = b;
}
