#include <algorithm>
#include <chrono>
#include <string>
#include "engine/enginev1.hpp"
#include "diagnostics.hpp"
#include "chess.hpp"
#include "utils.hpp"

using utils::DEB;
using utils::DBN;

chess::Move EngineV1::think() {
    this->diagnostics.new_move();

    auto time_begin = std::chrono::steady_clock::now();
    auto best_move = this->search_begin();
    auto time_end = std::chrono::steady_clock::now();

    diagnostics.write_custom_search_log(this->color, best_move, time_end, time_begin);
    return best_move;
}

chess::Move EngineV1::search_begin() {
    int player = this->color == chess::Color::WHITE ? 1 : -1;
    float best_evaluation = -player * this->POSITIVE_INFINITY;

    chess::Move best_move;
    auto legal_moves = utils::legal_moves(this->board);
    for (auto& move : legal_moves) {
        this->board->makeMove(move);
        float evaluation = -player * this->search(this->MAX_DEPTH - 1, this->NEGATIVE_INFINITY, this->POSITIVE_INFINITY, -player);
        this->board->unmakeMove(move);

        best_evaluation = utils::min_max(this->color, evaluation, best_evaluation);
        if (evaluation == best_evaluation) {
            best_move = move;
        }

        if (this->debug_mode) {
            auto branch = {move};
            this->diagnostics.path = this->diagnostics.long_path[utils::get_move_index(legal_moves, move)];
            this->diagnostics.path.insert(this->diagnostics.path.begin(), branch.begin(), branch.end());

            this->diagnostics.write_search_log(this->color, this->color, 0, best_move, best_evaluation, false);
            this->diagnostics.write_all_search_log(this->color, this->color, 0, move, evaluation, true);
            this->diagnostics.path = {};
        }
    }

    return best_move;
}

float EngineV1::search(int depth, float alpha, float beta, int player) {
    if (depth == 0) {
        auto ev = player * this->quiescense_search(alpha, beta);
        return ev;
    } else if (this->board->isGameOver().second == chess::GameResult::WIN) {
        return player * this->POSITIVE_INFINITY;
    } else if (this->board->isGameOver().second == chess::GameResult::LOSE) {
        return -player * this->POSITIVE_INFINITY;
    } else if (this->board->isGameOver().second == chess::GameResult::DRAW) {
        return 0.0;
    }

    ////// RECURSION //////
    auto legal_moves = utils::legal_moves(this->board);
    this->order_moves(legal_moves);

    chess::Move best_move;
    for (auto& move : legal_moves) {
        this->board->makeMove(move);
        float evaluation = -this->search(depth - 1, -beta, -alpha, -player);
        this->board->unmakeMove(move);

        ////// ALPHA BETA PRUNING //////
        alpha = std::max(alpha, evaluation);
        if (evaluation == alpha) {
            best_move = move;
        }

        if (beta <= alpha) {
            this->diagnostics.ab++;
            break; // *Snip*
        }
    }

    if (this->debug_mode)
        utils::path_generation(diagnostics, depth, legal_moves, best_move);
    return alpha;
}

// https://www.chessprogramming.org/Quiescence_Search
// This is to prevent the horizon effect
float EngineV1::quiescense_search(float alpha, float beta) {
    return this->evaluate_fen();
}

// https://www.chessprogramming.org/Move_Ordering
// https://www.chessprogramming.org/SEE_-_The_Swap_Algorithm
void EngineV1::order_moves(std::vector<chess::Move>& moves) {
    int score;
    for (auto& move : moves) {
        score = 0;
        chess::Square square_from = move.from();
        chess::Square square_to = move.to();
        chess::Piece piece_from = this->board->at(move.from());
        chess::Piece piece_to = this->board->at(move.to());
        // Not ensured that these are viable
        chess::PieceType piecetype_from = piece_from.type();
        chess::PieceType piecetype_to = piece_to.type();

        // Most Valuable Victim, Least Valuable Aggressor (MVVLVA)
        // Static Exchange Evaluation (SEE)
        if (piece_to != chess::Piece::NONE) {
            this->board->makeMove(move);
            score += this->see(square_to, this->board->sideToMove());
            this->board->unmakeMove(move);
        }

        move.setScore(score);
    }
 
    std::sort(moves.begin(), moves.end(), utils::compare_score);
}

// https://www.chessprogramming.org/Static_Exchange_Evaluation
// A recursive non-bitboard algorithm
int EngineV1::see(chess::Square& square, int side) {
    int value = 0;
    chess::Square smallest_square = this->smallest_attacker(square, side); 

    if (smallest_square != chess::Square::underlying::NO_SQ) {
        chess::Move move = chess::Move::make(smallest_square, square);
        this->board->makeMove(move);
        value = std::max(0,  this->get_piece_value(this->board->at(square)) - this->see(square, !side));
        this->board->unmakeMove(move);
    }

    return value;
} 

chess::Square EngineV1::smallest_attacker(chess::Square& square, int side) {
    chess::Bitboard attackers = chess::attacks::attackers(*this->board, side, square, this->board->occ());

    chess::Square smallest_square;
    chess::Piece smallest_attacker = chess::Piece::WHITEKING;
    for (int i = 0; i < 64; i++) {
        // Fancy chess language for if index i on the bitboard is a piece
        if (attackers & (1ULL << i)) {
            chess::Piece attacker = this->board->at(chess::Square(i));
            if (this->get_piece_value(attacker) < this->get_piece_value(smallest_attacker)) {
                smallest_attacker = attacker;
                smallest_square = chess::Square(i);
            }
        }
    }

    // King can't attack anything
    if (smallest_attacker == chess::Piece::WHITEKING) {
        return chess::Square::underlying::NO_SQ;
    } else {
        return smallest_square;
    }
}

// Revert this to utils?
int EngineV1::get_piece_value(chess::Piece p) {
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

int EngineV1::evaluate_fen() {
    this->diagnostics.positions_searched++;
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

EngineV1::EngineV1(chess::Color clr, int MAX_DEPTH) {
    this->MAX_DEPTH = MAX_DEPTH;
    this->color = clr;
    this->diagnostics = Diagnostics();
}

void EngineV1::set_board(chess::Board* b) {
    this->board = b;
}
