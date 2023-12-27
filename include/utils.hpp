#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include "chess.hpp"
#include "app.hpp"


namespace utils {

inline void DEB(std::string str) { std::cout << str << "\n"; }
inline void DEB(int i) { std::cout << i << "\n"; }
inline void DEB(chess::Move m) { std::cout << m << "\n"; }
inline void DBN(std::string str) { std::cout << str; }
inline void DBN(int i) { std::cout << i; }
inline void DBN(chess::Move m) { std::cout << m; }

inline bool is_clrw(chess::Color color) {
    if (color == chess::Color::WHITE) {
        return true;
    } else {
        return false;
    }
}

inline chess::Movelist legal_moves(chess::Board* b) {
    chess::Movelist moves;
    chess::movegen::legalmoves(moves, *b);
    return moves;
}

inline int get_piece_index(Piece value) {
    for (int i = 0; i < static_cast<int>(Piece::COUNT); i++) {
        if (static_cast<Piece>(i) == value)
            return i;
    }
    return -1; // If the value is not found
}

inline int get_move_index(chess::Movelist vec, chess::Move m) {
    for (int i = 0; i < vec.size(); i++) {
        if (vec[i] == m) return i;
    }

    return -1;
}

inline chess::Move is_in_moves(chess::Movelist moves, std::string currently_moving, std::string moving_to) {
    for (int i = 0; i <= moves.size(); i++) {
        if (moves[i].from() == chess::Square(currently_moving) && moves[i].to() == chess::Square(moving_to)) {
            return moves[i];
            break;
        } 
    }

    return chess::Move::NULL_MOVE;
}

inline int frti(chess::File file) {
    return std::stoi(std::to_string(file));
}

inline int frti(chess::Rank rank) {
    return std::stoi(std::to_string(rank));
}

class Line {
private:
    sf::Vertex vertices[4];

public:
    Line(sf::Vector2f startPos, sf::Vector2f endPos, sf::Color color, float thickness) {
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
