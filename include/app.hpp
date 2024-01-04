#pragma once

#include <SFML/Graphics.hpp>
#include "chess.hpp"
#include "engine/engine.hpp"

enum Colorscheme {
    def,
    sepia,
    purp,
    history,
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


class App {
private:
    sf::RenderWindow* window;
    sf::RenderWindow* scorewindow;
    sf::Event event;

    sf::VideoMode resolution;
    float cellsize;

    Engine* main_engine;
    Engine* secondary_engine;
    chess::Square moving_from;
    bool moving;

    chess::Board board;
    chess::Color playing_color;
    chess::Color engine_color;
    bool player_playing;

    std::vector<chess::Move> move_history;
    std::vector<std::vector<chess::Move>> arrow_history;
    int move_history_index = 0;
    bool history_mode = false;
    bool show_arrows = false;

    std::vector<sf::RectangleShape> boardshape;
    std::vector<sf::Texture> piece_textures;
    std::vector<sf::Sprite> piece_sprites;
    Colorscheme colorscheme = Colorscheme::def;

public:
    App(
        Engine* engine1,
        Engine* engine2,
        chess::Color playing_color,
        bool player_playing,
        bool scorewindow
    );
    ~App();

    void run();
    void move_piece();
    void draw_position();
    void draw_board();
    void handle_events();
    void handle_moves();
};
