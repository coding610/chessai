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

    float alpha = std::numeric_limits<float>::min();
    float beta = std::numeric_limits<float>::max();

    chess::Move best_move;
    float best_value = this->worst_value;

    chess::Movelist legal_moves = utils::legal_moves(this->board);
    for (auto& move : legal_moves) {
        DBN(this->color);
        DBN(" Progress: ");
        DBN(100 * (utils::get_move_index(legal_moves, move) + 1) / legal_moves.size()); DBN("%");
        DBN("\n");

        this->board->makeMove(move);
        float value = this->search(this->board, 1, alpha, beta, !utils::is_clrw(this->color));
        this->board->unmakeMove(move);

        if (
            (this->color == chess::Color::WHITE && value > best_value) ||
            (this->color == chess::Color::BLACK && value < best_value)
        ) {
            best_value = value;
            best_move = move;
        } 
    }

    this->best_path.push_back(best_move);

    DBN("Positions_searched: "); DEB(this->positions_searched);
    DBN("Move choosen: "); DEB(best_move);
    DBN("Engines evaluation: "); DEB(best_value);
    DBN("AB: "); DEB(this->ab);
    if (this->color == chess::Color::BLACK) {
        DEB(legal_moves.size());
        DEB(this->MAX_DEPTH);
    }

    return best_move;
}

float Engine::search(chess::Board* b, int depth, float alpha, float beta, bool clrw) {
    ////// NO FURTHER RECURSION HANDLELING //////
    if (depth > this->MAX_DEPTH) {
        return this->evaluate_fen(b->getFen());
    } else  if (b->isGameOver().second == chess::GameResult::WIN) {
        return this->best_value;
    } else if (b->isGameOver().second == chess::GameResult::LOSE) {
        return this->worst_value;
    } else if (b->isGameOver().second == chess::GameResult::DRAW) {
        return 0.0;
    }

    float best_value = clrw ? std::numeric_limits<float>::min() : std::numeric_limits<float>::max();
    for (auto& move : utils::legal_moves(b)) {
        ////// RECURSION //////
        b->makeMove(move);
        float value = this->search(b, depth + 1, alpha, beta, !clrw);
        b->unmakeMove(move);

        ////// ALPHA BETA PRUNING //////
        if (!this->ab_pruning) continue;
        if (clrw) {
            best_value = std::max(best_value, value);
            alpha = std::max(alpha, value);
        } else {
            best_value = std::min(best_value, value);
            beta = std::min(alpha, value);
        }
        if (beta <= alpha) {
            this->ab++;
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
            case ' ': return score - score;
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
        this->worst_value = -std::numeric_limits<float>::max();
    } else {
        this->best_value = -std::numeric_limits<float>::max();
        this->worst_value = std::numeric_limits<float>::max();
    }
}

void Engine::setBoard(chess::Board* b) {
    this->board = b;
}
