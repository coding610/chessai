#include <cstdlib>
#include <string>
#include <limits>
#include "chess.hpp"
#include "engine.hpp"
#include "utils.hpp"

using utils::DEB;
using utils::DBN;

chess::Move Engine::think() {
    this->positions_searched = 0;
    this->ab = 0;
    this->best_path = {};

    float alpha = -std::numeric_limits<float>::max();
    float beta = std::numeric_limits<float>::max();

    chess::Move best_move;
    float best_value = this->worst_value;

    chess::Movelist legal_moves = utils::legal_moves(*this->board);
    for (auto& move : legal_moves) {
        std::cout << "----------Color: " << this->color << "---------- // " << "progress: "
                  << 100 * (utils::get_move_index(legal_moves, move) + 1) / legal_moves.size()
                  << "%\n";

        this->board->makeMove(move);
        float value = this->search(*this->board, 2, alpha, beta, utils::is_clrw(this->board->sideToMove()));
        this->board->unmakeMove(move);

        if (this->color == chess::Color::WHITE) {
            best_value = std::max(best_value, value);
        } else {
            best_value = std::min(best_value, value);
        } if (best_value == value) {
            best_move = move;
        }
    }

    this->best_path.push_back(best_move);

    DBN("\nPositions_searched: "); DEB(this->positions_searched);
    DBN("Move choosen: "); DEB(best_move);
    DBN("Engines evaluation: "); DEB(best_value);
    DBN("AB: "); DEB(this->ab);
    DBN("AB2: "); DEB(this->ab2);
    DBN("Without AB num pos: "); DEB(this->ab2 + this->positions_searched);
    DBN("Recursion depth: "); DEB(this->recursion_depth);
    DBN("Total searched moves: "); DEB(this->total_searched_moves);
    DBN("Depth3: "); DEB(this->depth3);
    DEB("");

    return best_move;
}

float Engine::search(chess::Board& b, int depth, float alpha, float beta, bool clrw) {
    this->recursion_depth++;

    const float best_possible_value = clrw ? std::numeric_limits<float>::max() : -std::numeric_limits<float>::max();
    const float worst_possible_value = -best_possible_value;
    float best_value = worst_possible_value;

    ////// NO FURTHER RECURSION HANDLELING //////
    if (depth > this->MAX_DEPTH) {
        return this->evaluate_fen(b.getFen());
    } else if (b.isGameOver().second == chess::GameResult::WIN) {
        return best_possible_value;
    } else if (b.isGameOver().second == chess::GameResult::LOSE) {
        return worst_possible_value;
    } else if (b.isGameOver().second == chess::GameResult::DRAW) {
        return 0.0;
    }

    chess::Movelist legal_moves = utils::legal_moves(b);
    if (depth == 3) this->total_searched_moves += legal_moves.size();

    int i = 0;
    for (auto& move : legal_moves) {
        ////// RECURSION //////
        b.makeMove(move);
        float value = this->search(b, depth + 1, beta, alpha, !clrw);
        b.unmakeMove(move);

        ////// ALPHA BETA PRUNING //////
        if (clrw) {
            best_value = std::max(best_value, value);
            if (!this->ab_pruning) continue;
            alpha = std::max(alpha, value);
        } else {
            best_value = std::min(best_value, value);
            if (!this->ab_pruning) continue;
            beta = std::min(beta, value);
        }

        if (beta <= alpha) {
            this->ab++;
            this->ab2 += utils::legal_moves(b).size() - utils::get_move_index(legal_moves, move);
            return best_value; // * Snip *
        }
    }

    return best_value;
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
        this->best_value = std::numeric_limits<float>::max();
        this->worst_value = -this->best_value;
    } else {
        this->best_value = -std::numeric_limits<float>::max();
        this->worst_value = -this->best_value;
    }
}

void Engine::setBoard(chess::Board* b) {
    this->board = b;
}
