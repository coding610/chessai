#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include "chess.hpp"
#include "app.hpp"


namespace utils {

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
