#include <string>
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
        alpha = this->POSITIVE_INFINITY;
        beta = this->NEGATIVE_INFINITY;
    } else {
        alpha = this->NEGATIVE_INFINITY;
        beta = this->POSITIVE_INFINITY;
    }

    chess::Move best_move;
    float best_evaluation = this->worst_value;
    chess::Movelist legal_moves = utils::legal_moves(*this->board);
    for (auto& move : legal_moves) {
        std::cout << "Progress: "
                  << 100 * (utils::get_move_index(legal_moves, move) + 1) / legal_moves.size()
                  << "%\r" << std::flush;

        this->board->makeMove(move);
        float evaluation = this->search(*this->board, 2, alpha, beta, utils::is_clrw(this->board->sideToMove()));
        this->board->unmakeMove(move);

        if (this->color == chess::Color::WHITE) {
            best_evaluation = std::max(best_evaluation, evaluation);
        } else {
            best_evaluation = std::min(best_evaluation, evaluation);
        } if (evaluation == best_evaluation) {
            best_move = move;
        }
    }

    ////// BEST PATH //////
    this->total_path = this->best_path[utils::get_move_index(legal_moves, best_move)];

    DBN("Positions searched: "); DEB(this->positions_searched);
    DBN("Engines evaluation: "); DEB(best_evaluation);
    DEB("");

    return best_move;
}

float Engine::search(chess::Board& b, int depth, float alpha, float beta, bool clrw) {
    const float best_possible_evaluation = clrw ? this->POSITIVE_INFINITY: this->NEGATIVE_INFINITY;
    const float worst_possible_evaluation = clrw ? this->NEGATIVE_INFINITY: this->POSITIVE_INFINITY;

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
        if (clrw) {
            if (evaluation > best_evaluation) {
                best_evaluation = evaluation;
                best_move = move;
            }
        } else {
            if (evaluation < best_evaluation) {
                best_evaluation = evaluation;
                best_move = move;
            }
        }

        ////// ALPHA BETA PRUNING //////
        if (this->ab_pruning) {
            alpha = std::max(alpha, evaluation);
            if (beta <= alpha) {
                break;
            }
        }
    }

    if (depth == MAX_DEPTH) { // best_path.size() == 0
        this->best_path.push_back({best_move});
        return best_evaluation;
    }

    ////// REMOVE WRONG PATH //////
    std::vector<std::vector<chess::Move>> depth_path = {};
    for (auto& path : this->best_path) {
        if (path.size() >= MAX_DEPTH - depth + 1) {
            depth_path.push_back(path);
        }
    }

    ////// PUSH BACK NEW VECTOR //////
    int index = utils::get_move_index(legal_moves, best_move);
    if (index == -1) best_move = legal_moves[0];

    std::vector<chess::Move> branch = this->best_path[depth_path.size() + utils::get_move_index(legal_moves, best_move)];
    branch.push_back(best_move);
    depth_path.push_back(branch);
    utils::copy_and_erease_vector(&this->best_path, depth_path);

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
    DEB(this->POSITIVE_INFINITY);
    DEB(this->NEGATIVE_INFINITY);
    this->MAX_DEPTH = MAX_DEPTH;
    this->ab_pruning = ab_pruning;

    this->color = color;
    if (color == chess::Color::WHITE) {
        this->best_value = this->POSITIVE_INFINITY;
        this->worst_value = this->NEGATIVE_INFINITY;
    } else {
        this->best_value = this->NEGATIVE_INFINITY;
        this->worst_value = this->POSITIVE_INFINITY;
    }
}

void Engine::setBoard(chess::Board* b) {
    this->board = b;
}
