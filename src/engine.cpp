#include <algorithm>
#include <chrono>
#include <string>
#include <iostream>
#include <fstream>
#include "chess.hpp"
#include "engine.hpp"
#include "utils.hpp"

using utils::DEB;
using utils::DBN;

chess::Move Engine::think() {
    this->engine_move_index++;
    this->positions_searched = 0;
    this->ab = 0;

    auto time_begin = std::chrono::steady_clock::now();
    auto best_move = this->search_begin();
    auto time_end = std::chrono::steady_clock::now();

    std::ofstream diagnostics;
    diagnostics.open("diagnostics/search_logs.txt", std::ios::app);
    diagnostics << "\n--- Engine Evaluation Finished -- Current Move: " << this->engine_move_index << " ---\n"
                << "--- Position searched: " << this->positions_searched << " ---\n"
                << "--- αβ Pruning Index: " << this->ab << " ---\n"
                << "--- Duration: "
                << std::chrono::duration_cast<std::chrono::milliseconds>(time_end - time_begin).count()
                << "ms ---\n"
                << "--- Final Move Chooosen: " << best_move << " ---\n\n";
    diagnostics.close();

    return best_move;
}

chess::Move Engine::search_begin() {
    chess::Move best_move;
    float best_evaluation = this->color == chess::Color::WHITE ? this->NEGATIVE_INFINITY : this->POSITIVE_INFINITY;
    auto legal_moves = utils::legal_moves(this->board);
    this->order_moves(legal_moves);

    for (auto& move : legal_moves) {
        this->board->makeMove(move);
        float evaluation = this->search(this->MAX_DEPTH - 1, this->NEGATIVE_INFINITY, this->POSITIVE_INFINITY);
        this->board->unmakeMove(move);

        if (this->color == chess::Color::WHITE) {
            best_evaluation = std::max(evaluation, best_evaluation);
        } else {
            best_evaluation = std::min(evaluation, best_evaluation);
        } if (evaluation == best_evaluation) {
            best_move = move;
        }

        utils::write_search_log(this->color, this->color, 0, this->ab, best_move, best_evaluation, this->engine_move_index);
    }

    return best_move;
}

float Engine::search( int depth, float alpha, float beta) {
    if (depth == 0) {
        auto ev = this->quiescense_search(alpha, beta);
        return ev;
    } else if (this->board->isGameOver().second == chess::GameResult::WIN) {
        return BEST_POSSIBLE_VALUE;
    } else if (this->board->isGameOver().second == chess::GameResult::LOSE) {
        return WORST_POSSIBLE_VALUE;
    } else if (this->board->isGameOver().second == chess::GameResult::DRAW) {
        return 0.0;
    }

    ////// RECURSION //////
    auto legal_moves = utils::legal_moves(this->board);
    this->order_moves(legal_moves);

    chess::Move best_move;
    for (auto& move : legal_moves) {
        this->board->makeMove(move);
        float evaluation = this->search(depth - 1, -beta, -alpha);
        this->board->unmakeMove(move);

        ////// ALPHA BETA PRUNING //////
        alpha = std::max(alpha, evaluation);
        if (evaluation == alpha) {
            best_move = move;
        }

        if (this->ab_pruning) {
            if (beta <= alpha) {
                this->ab++;
                return alpha;
            }
        }

        ////// DIAGNOSTICS //////
        if (this->write_low_depth_search_log) {
            utils::write_ld_search_log(this->color, this->board->sideToMove(), this->MAX_DEPTH - depth, this->ab, best_move, alpha, this->engine_move_index);
        }
    }

    return alpha;
}

// https://www.chessprogramming.org/Quiescence_Search
float Engine::quiescense_search(
    float alpha,
    float beta
) {
    ////// STANDING PAT //////
    float evaluation = this->evaluate_fen();
    // if (evaluation >= beta) {
    //     return beta;
    // } else {
    //     alpha = std::max(alpha, evaluation);
    // }
    //
    // for (auto& m : )


    return evaluation;
}

void Engine::order_moves(std::vector<chess::Move>& moves) {
    int score;
    for (auto& move : moves) {
        score = 0;
        chess::Piece piece_from = this->board->at( move.from() );
        chess::Piece piece_attacking = this->board->at( move.to() );
        chess::Square square_attacking = move.to();


        // Rewarding and punishing capturing pieces
        if (piece_attacking != chess::Piece::NONE) {
            // Sebastian Lauge did without parantathes
            // Might need to change this in the future
            score += 10 * this->get_piece_value(piece_from) - this->get_piece_value(piece_attacking);
        }

        // Promoting a pawn is likely to be good
        if (board->sideToMove() == chess::Color::WHITE) {
            if (move.to().rank() == chess::Rank::RANK_8) score += this->get_piece_value(piece_attacking);
        } else {
            if (move.to().rank() == chess::Rank::RANK_1) score += this->get_piece_value(piece_attacking);
        }

        // Punishing pieces from moving to a square attacked by a pawn
        this->board->makeMove(move);
        if (utils::is_in_vector(square_attacking, utils::legal_moves_piece(this->board, chess::PieceGenType::PAWN))) {
            score -= this->get_piece_value(piece_from);
        }
        this->board->unmakeMove(move);

        move.setScore(score);
    }

    // Order legal_moves by its score value
    std::sort(moves.begin(), moves.end(), utils::compare_score);
}

int Engine::get_piece_value(chess::Piece p) {
    switch (p) {
        case 0: return this->PAWN_VALUE;
        case 1: return this->KNIGHT_VALUE;
        case 2: return this->BISHOP_VALUE;
        case 3: return this->ROOK_VALUE;
        case 4: return this->QUEEN_VALUE;
        case 5: return this->POSITIVE_INFINITY;
        case 6: return this->PAWN_VALUE;
        case 7: return this->KNIGHT_VALUE;
        case 8: return this->BISHOP_VALUE;
        case 9: return this->ROOK_VALUE;
        case 10: return this->QUEEN_VALUE;
        case 11: return this->POSITIVE_INFINITY;
        default:
            return -1;
    }
}

float Engine::evaluate_fen() {
    positions_searched++;
    std::string fen = this->board->getFen();

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
}

void Engine::setBoard(chess::Board* b) {
    this->board = b;
}
