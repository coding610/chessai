#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include "chess.hpp"
#include "engine.hpp"

enum Colorscheme {
    def,
    sepia,
};

enum Piece {
    r,
    n,
    b,
    q,
    k,
    p,
    R,
    N,
    B,
    Q,
    K,
    P,
    COUNT
};


int getEnumIndex(Piece value);

class App {
private:
    sf::RenderWindow* window;
    sf::RenderWindow* scorewindow;
    sf::Event event;

    sf::VideoMode resolution;
    float cellsize;

    Engine* engine1;
    Engine* engine2;

    bool game_finished;
    chess::Board board;
    chess::Color playing_color;
    chess::Color engine_color;

    std::vector<chess::Move> move_history;
    int move_history_index = 0;
    bool history_mode = false;
    bool show_arrows = false;

    std::vector<sf::RectangleShape> boardshape;
    std::vector<sf::Texture> piece_textures;
    std::vector<sf::Sprite> piece_sprites;
    Colorscheme colorscheme;

    std::string currently_moving;

public:
    App(Engine* engine1, Engine* engine2, chess::Color playing_color, bool scorewindow);
    ~App();

    void run();
    void move_piece();
    void draw_position();
    void draw_board();
    void display_arrows(Engine* engine);
};
