#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include "chess.hpp"
#include "app.hpp"

namespace utils {

inline bool compare_score(chess::Move m1, chess::Move m2) {
    return (m1.score() > m2.score());
}

inline void DEB(std::string str) { std::cout << str << "\n"; }
inline void DEB(int i) { std::cout << i << "\n"; }
inline void DEB(chess::Move m) { std::cout << m << "\n"; }
template<typename T>
inline void DEB(std::vector<std::vector<T>> vec) {
    for (auto& a : vec) {
        DEB("BRANCH: ");
        for (auto& b : a) DEB(b);
    }
}
template<typename T>
inline void DEB(std::vector<T> vec) {
    for (auto& a : vec) {
        DEB(a);
    }
}
inline void DBN(std::string str) { std::cout << str; }
inline void DBN(int i) { std::cout << i; }
inline void DBN(chess::Move m) { std::cout << m; }

inline bool is_game_over(chess::Board& b) {
    return b.isGameOver().second != chess::GameResult::NONE;
}

inline std::string square_to_san(chess::Square square) {
    char fileChar = 'a' + static_cast<char>(square.file());
    int rankNumber = static_cast<int>(square.rank()) + 1;
    return std::string{fileChar} + std::to_string(rankNumber);
}

template<typename T>
inline void copy_and_erease_vector(std::vector<T>* ereasev, std::vector<T>& copyv) {
    *ereasev = {};
    for (int i = 0; i < copyv.size(); i++) {
        ereasev->push_back(copyv[i]);
    }
}

inline int get_piece_index(Piece value) {
    for (int i = 0; i < static_cast<int>(Piece::COUNT); i++) {
        if (static_cast<Piece>(i) == value)
            return i;
    }
    return -1; // If the value is not found
}

inline std::vector<chess::Move> cmvl_to_v(chess::Movelist mvl) {
    std::vector<chess::Move> v;
    for (auto& m : mvl) {
        v.push_back(m);
    }

    return v;
}

inline std::vector<chess::Square> legal_moves_piece(chess::Board* b, chess::PieceGenType p) {
    chess::Movelist moves;
    chess::movegen::legalmoves(moves, *b, chess::PieceGenType::PAWN);

    std::vector<chess::Square> v;
    for (auto& m : moves) {
        v.push_back(m.to());
    }
    return v;
}

inline std::vector<chess::Move> legal_moves(chess::Board* b, bool generate_captures = false) {
    chess::Movelist legal_moves;
    chess::movegen::legalmoves(legal_moves, *b);
    // Might need to change this for speed
    std::vector<chess::Move> legal_moves_v;
    for (auto& move : legal_moves) {
        if (generate_captures) {
            if (b->isCapture(move)) {
                legal_moves_v.push_back(move);
            }
        } else {
            legal_moves_v.push_back(move);
        }
    }

    return legal_moves_v;
}

template <typename T>
inline bool is_in_vector(T& move, std::vector<T> moves) {
    for (auto& m : moves) {
        if (m == move) {
            return true;
        }
    }

    return false;
}

inline bool is_legal(chess::Move& move, chess::Board* b) {
    auto legal_moves = utils::legal_moves(b);

    return utils::is_in_vector(move, utils::legal_moves(b));
}

// https://stackoverflow.com/questions/17032970/clear-data-inside-text-file-in-c
inline void clear_search_log() {
    std::ofstream ofs;
    ofs.open("diagnostics/search_logs.txt", std::ofstream::out | std::ofstream::trunc);
    ofs.close();

    std::ofstream ofs2;
    ofs2.open("diagnostics/depth_search_logs.txt", std::ofstream::out | std::ofstream::trunc);
    ofs2.close();

    std::ofstream ofs3;
    ofs3.open("diagnostics/low_depth_search_logs.txt", std::ofstream::out | std::ofstream::trunc);
    ofs3.close();
}

inline void write_search_log(chess::Color engine_color, chess::Color search_color, int depth, int ab, chess::Move best_move, float evaluation, int index) {
    std::ofstream dia_file;
    dia_file.open("diagnostics/depth_search_logs.txt", std::ios::app);

    // --- Engine Color: WHITE -- Search Color: BLACK -- Depth: 3 -- Best_move: a2a3 -- Evaluation: -1.0 ---
    dia_file << "--- Engine Color: " << engine_color
             << " -- Search Color: " << search_color
             << " -- Depth: "        << depth
             << " -- Current αβ pruning index: " << ab
             << " -- Best Move: "    << best_move
             << " -- Evaluation: "   << evaluation
             << " -- Move Index: " << index
             << "\n";

    dia_file.close();
}

inline void write_ld_search_log(chess::Color engine_color, chess::Color search_color, int depth, int ab, chess::Move best_move, float evaluation, int index) {
    std::ofstream dia_file;
    dia_file.open("diagnostics/low_depth_search_logs.txt", std::ios::app);

    // --- Engine Color: WHITE -- Search Color: BLACK -- Depth: 3 -- Best_move: a2a3 -- Evaluation: -1.0 ---
    dia_file << "--- Engine Color: " << engine_color
             << " -- Search Color: " << search_color
             << " -- Depth: "        << depth
             << " -- Current αβ pruning index: " << ab
             << " -- Best Move: "    << best_move
             << " -- Evaluation: "   << evaluation
             << " -- Move Index: " << index
             << "\n";

    dia_file.close();
}

inline void write_custom_search_log(std::string message) {
    std::ofstream dia_file;
    dia_file.open("diagnostics/search_logs.txt", std::ios::app);

    dia_file << message;
    dia_file.close();
}

class Line {
private:
    sf::Vertex vertices[4];

public:
    sf::Vector2f start_pos;
    sf::Vector2f end_pos;

public:
    Line(sf::Vector2f startPos, sf::Vector2f endPos, sf::Color color, float thickness) {
        this->start_pos = startPos;
        this->end_pos = endPos;

        sf::Vector2f direction = endPos - startPos;
        float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        sf::Vector2f unitDirection = direction / length;
        sf::Vector2f perpendicular(-unitDirection.y, unitDirection.x);

        sf::Vector2f offset = (thickness / 2.0f) * perpendicular;

        vertices[0].position = startPos + offset;
        vertices[1].position = endPos + offset;
        vertices[2].position = endPos - offset;
        vertices[3].position = startPos - offset;

        for (int i = 0; i < 4; ++i) {
            vertices[i].color = color;
        }
    }

    void draw(sf::RenderWindow& window) {
        window.draw(vertices, 4, sf::Quads);
    }


};

}
