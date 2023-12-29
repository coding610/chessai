#include <string>
#include <limits>
#include "chess.hpp"
#include "engine.hpp"
#include "utils.hpp"

using utils::DEB;
using utils::DBN;


chess::Move Engine::think() {
    std::cout << "---------------- Engine Color: " << this->color << "----------------\n";

    this->positions_searched = 0;
    this->best_path = {};
    this->total_path = {};

    float alpha, beta;
    if (this->color == chess::Color::WHITE) {
        alpha = (float) std::numeric_limits<int>::max();
        beta = (float) -std::numeric_limits<int>::max();
    } else {
        alpha = (float) -std::numeric_limits<int>::max();
        beta = (float) std::numeric_limits<int>::max();
    }

    chess::Move best_move;
    float best_value = this->worst_value;

    chess::Movelist legal_moves = utils::legal_moves(*this->board);
    for (auto& move : legal_moves) {
        std::cout << "Progress: "
                  << 100 * (utils::get_move_index(legal_moves, move) + 1) / legal_moves.size()
                  << "%\n" << std::flush;

        this->board->makeMove(move);
        float value = this->search(*this->board, 2, alpha, beta, utils::is_clrw(this->board->sideToMove()));
        this->board->unmakeMove(move);

        if (this->color == chess::Color::WHITE) {
            best_value = std::max(best_value, value);
        } else {
            best_value = std::min(best_value, value);
        } if (value == best_value) {
            best_move = move;
        }
    }

    DBN("Positions searched: "); DEB(this->positions_searched);
    DBN("Engines evaluation: "); DEB(best_value);
    DBN("AB: "); DEB(this->ab);
    DEB(this->total_path.size());
    DEB("");

    return best_move;
}

float Engine::search(chess::Board& b, int depth, float alpha, float beta, bool clrw) {
    const float best_possible_evaluation = clrw ? std::numeric_limits<float>::max() : -std::numeric_limits<float>::max();
    const float worst_possible_evaluation = -best_possible_evaluation;

    ////// NO FURTHER RECURSION HANDLELING //////
    if (depth > this->MAX_DEPTH) {
        return this->evaluate_fen(b.getFen());
    } else if (b.isGameOver().second == chess::GameResult::WIN) {
        return best_possible_evaluation;
    } else if (b.isGameOver().second == chess::GameResult::LOSE) {
        return worst_possible_evaluation;
    } else if (b.isGameOver().second == chess::GameResult::DRAW) {
        return 0.0;
    }

    float best_evaluation = worst_possible_evaluation;
    chess::Move best_move = chess::Move::NULL_MOVE;

    chess::Movelist legal_moves = utils::legal_moves(b);
    for (auto& move : legal_moves) {
        ////// RECURSION //////
        b.makeMove(move);
        float evaluation = this->search(b, depth + 1, -beta, -alpha, !clrw);
        b.unmakeMove(move);

        ////// EVALUATION //////
        best_evaluation = clrw ? std::max(best_evaluation, evaluation) : std::min(best_evaluation, evaluation);
        if (evaluation == best_evaluation && move != chess::Move::NULL_MOVE) {
            best_move = move;
        }

        ////// ALPHA BETA PRUNING //////
        if (!this->ab_pruning) continue;

        alpha = std::max(alpha, evaluation);
        if (beta <= alpha) {
            this->best_path.push_back({best_move});
            this->ab++;
            return best_evaluation;
        }
    }

    if (depth == MAX_DEPTH) { // best_path.size() == 0
        this->best_path.push_back({best_move});
        return best_evaluation;
    }

    ////// REMOVE WRONG PATH //////
    std::vector<std::vector<chess::Move>> depth_path = {};
    for (auto& path : this->best_path) {
        if (path.size() == MAX_DEPTH - depth + 1) {
            depth_path.push_back(path);
        }
    }


    ////// PUSH BACK NEW VECTOR //////
    std::vector<chess::Move> branch = this->best_path[depth_path.size() + utils::get_move_index(legal_moves, best_move)];
    branch.push_back(best_move);
    depth_path.push_back(branch);

    this->best_path = {};
    for (int i = 0; i < depth_path.size(); i++) {
        this->best_path.push_back(depth_path[i]);
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

Engine::Engine(chess::Color color, bool ab_pruning, int MAX_DEPTH) {
    this->MAX_DEPTH = MAX_DEPTH;
    this->ab_pruning = ab_pruning;

    this->color = color;
    if (color == chess::Color::WHITE) {
        this->best_value = (float) std::numeric_limits<int>::max();
        this->worst_value = (float) -std::numeric_limits<int>::max();
    } else {
        this->best_value = (float) -std::numeric_limits<int>::max();
        this->worst_value = (float) std::numeric_limits<int>::max();
    }
}

void Engine::setBoard(chess::Board* b) {
    this->board = b;
}
