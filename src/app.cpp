#include <SFML/Graphics.hpp>
#include <string>
#include "chess.hpp"
#include "app.hpp"
#include "engine.hpp"
#include "utils.hpp"

using utils::DEB;
using utils::DBN;

void App::run() {
    while (this->window->isOpen()) {
        if (this->board.is_game_over() && this->history_mode == false) {
            DEB("Game Over!");
            this->history_mode = true;
        }

        if (!this->history_mode) {
            this->handle_moves(); 
        }

        this->handle_events();
        this->draw_board();
        this->draw_position();
        // this->display_arrows();
        this->window->display();
    }
}

void App::handle_moves() {
    chess::Move move = chess::Move::from_uci("a1a1");
    if (this->player_playing) {
        if (this->board.turn == this->playing_color) {
            this->move_piece();
            return;
        } else {
            if (this->engine_color == chess::WHITE) {
                move = this->enginew->think();
            } else {
                move = this->engineb->think();
            }
        }
    } else {
        if (this->board.turn == chess::WHITE) {
            move = this->enginew->think();
        } else {
            move = this->engineb->think();
        }
    }

    this->move_history_index++;
    this->move_history.push_back(move);
    this->board.push(move);
}

void App::handle_events() {
    while (this->window->pollEvent(this->event)) {
        switch (this->event.type) {
            case sf::Event::Closed:
                this->window->close();
                break;

            case sf::Event::KeyPressed:
                switch (this->event.key.code) {
                    case sf::Keyboard::A:
                        this->show_arrows = !this->show_arrows;
                        break;

                    case sf::Keyboard::Left:
                        if (this->move_history_index == 0)
                            break;

                        this->history_mode = true;
                        this->move_history_index--;
                        this->colorscheme = Colorscheme::history;

                        // this->board.pop(this->move_history[this->move_history_index]);
                        this->board.pop();
                        break;

                    case sf::Keyboard::Right:
                        if (this->move_history_index == this->move_history.size()) {
                            break;
                        } else if (this->move_history_index == this->move_history.size() - 1) {
                            this->history_mode = false;
                            this->colorscheme = Colorscheme::def;
                        }

                        this->board.push(this->move_history[this->move_history_index]);
                        this->move_history_index++;
                        break;

                    default: break;
            }

            default: break;
        }
    }
}

void App::move_piece() {
    if (!this->moving && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        sf::Vector2<int> mousepos = sf::Mouse::getPosition(*(this->window));

        this->moving = true;
        this->moving_from = chess::Square(
            8 * std::floor(mousepos.x / this->cellsize) +
            std::floor(mousepos.y / this->cellsize)
        );

    } else if (this->moving && !(sf::Mouse::isButtonPressed(sf::Mouse::Left))) {
        sf::Vector2<int> mousepos = sf::Mouse::getPosition(*(this->window));
        auto moving_to = chess::Square(
            8 * std::floor(mousepos.x / this->cellsize) +
            std::floor(mousepos.y / this->cellsize)
        );

        chess::Move move = chess::Move(moving_from, moving_to);
        if (!this->board.is_legal(move)) {
            return;
        }

        this->move_history_index++;
        this->move_history.push_back(move);
        this->board.push(move);
    }
}

void App::draw_board() {
    for (int i = 0; i <= 8; i++) {
        for (int j = 0; j <= 8; j++) {
            sf::RectangleShape r(sf::Vector2f(this->cellsize, this->cellsize));
            r.setPosition(i * this->cellsize, j * this->cellsize);

            switch (this->colorscheme) {
                case def:
                    if ((i + j) % 2 == 0) r.setFillColor(sf::Color(120, 141, 89));
                    else r.setFillColor(sf::Color(227, 227, 204));
                    break;
                case sepia:
                    if ((i + j) % 2 == 0) r.setFillColor(sf::Color(0, 0, 0));
                    else r.setFillColor(sf::Color(255, 255, 255));
                    break;
                case purp:
                    if ((i + j) % 2 == 0) r.setFillColor(sf::Color(240, 241, 240));
                    else r.setFillColor(sf::Color(129, 120, 181));
                    break;
                case history:
                    if ((i + j) % 2 == 0) r.setFillColor(sf::Color(80, 101, 59));
                    else r.setFillColor(sf::Color(187, 187, 164));
                    break;
            }

            this->boardshape.push_back(r);
        }
    }

    for (sf::RectangleShape r : this->boardshape) {
        this->window->draw(r);
    }
}

void App::draw_position() {
    std::string fen = this->board.fen();

    int line = 0;
    int column = 0;
    for (char& c : fen) {
        switch (c) {
            case ' ': return;
            case '/': line++; column = 0; break;
            case 'r':
                this->piece_sprites[utils::get_piece_index(r)].setPosition(column * this->cellsize, line * this->cellsize);
                this->window->draw(this->piece_sprites[utils::get_piece_index(r)]); column++; break;
            case 'n':
                this->piece_sprites[utils::get_piece_index(n)].setPosition(column * this->cellsize, line * this->cellsize);
                this->window->draw(this->piece_sprites[utils::get_piece_index(n)]); column++; break;
            case 'b':
                this->piece_sprites[utils::get_piece_index(b)].setPosition(column * this->cellsize, line * this->cellsize);
                this->window->draw(this->piece_sprites[utils::get_piece_index(b)]); column++; break;
            case 'q':
                this->piece_sprites[utils::get_piece_index(q)].setPosition(column * this->cellsize, line * this->cellsize);
                this->window->draw(this->piece_sprites[utils::get_piece_index(q)]); column++; break;
            case 'k':
                this->piece_sprites[utils::get_piece_index(k)].setPosition(column * this->cellsize, line * this->cellsize);
                this->window->draw(this->piece_sprites[utils::get_piece_index(k)]); column++; break;
            case 'p':
                this->piece_sprites[utils::get_piece_index(p)].setPosition(column * this->cellsize, line * this->cellsize);
                this->window->draw(this->piece_sprites[utils::get_piece_index(p)]); column++; break;
            case 'R':
                this->piece_sprites[utils::get_piece_index(R)].setPosition(column * this->cellsize, line * this->cellsize);
                this->window->draw(this->piece_sprites[utils::get_piece_index(R)]); column++; break;
            case 'N':
                this->piece_sprites[utils::get_piece_index(N)].setPosition(column * this->cellsize, line * this->cellsize);
                this->window->draw(this->piece_sprites[utils::get_piece_index(N)]); column++; break;
            case 'B':
                this->piece_sprites[utils::get_piece_index(B)].setPosition(column * this->cellsize, line * this->cellsize);
                this->window->draw(this->piece_sprites[utils::get_piece_index(B)]); column++; break;
            case 'Q':
                this->piece_sprites[utils::get_piece_index(Q)].setPosition(column * this->cellsize, line * this->cellsize);
                this->window->draw(this->piece_sprites[utils::get_piece_index(Q)]); column++; break;
            case 'K':
                this->piece_sprites[utils::get_piece_index(K)].setPosition(column * this->cellsize, line * this->cellsize);
                this->window->draw(this->piece_sprites[utils::get_piece_index(K)]); column++; break;
            case 'P':
                this->piece_sprites[utils::get_piece_index(P)].setPosition(column * this->cellsize, line * this->cellsize);
                this->window->draw(this->piece_sprites[utils::get_piece_index(P)]); column++; break;
        }
        if (std::isdigit(c))
            column += c - '0'; // - '0' is just a trick to converting c to a int
    }
}

// void App::display_arrows() {
//     if (!this->show_arrows) return;
//     
//     for (auto& e : {this->enginew, this->engineb}) {
//         if (e == nullptr)
//             return;
//
//         for (int i = 0; i < this->engine1->total_path.size(); i++) {
//             auto move = this->engine1->total_path[i];
//             auto color = i * 255 / this->engine1->total_path.size();
//
//             utils::Line line(
//                 sf::Vector2f(
//                     utils::frti(move.from().file()) * this->cellsize + this->cellsize / 2,
//                     7.5 * this->cellsize - this->cellsize * utils::frti(move.from().rank())
//                 ),
//                 sf::Vector2f(
//                     utils::frti(move.to().file()) * this->cellsize + this->cellsize / 2,
//                     7.5 * this->cellsize - this->cellsize * utils::frti(move.to().rank())
//                 ),
//                 sf::Color(color, color, color),
//                 10.0
//             );
//
//             sf::CircleShape from;
//             from.setFillColor(sf::Color(255, 255, 255));
//             from.setOutlineColor(sf::Color(0, 0, 0));
//             from.setOutlineThickness(2);
//             from.setRadius(10);
//             from.setPosition(
//                 line.start_pos.x - from.getRadius(),
//                 line.start_pos.y - from.getRadius()
//             );
//
//             sf::CircleShape to;
//             to.setFillColor(sf::Color(0, 0, 0));
//             to.setOutlineColor(sf::Color(255, 255, 255));
//             to.setOutlineThickness(2);
//             to.setRadius(10);
//             to.setPosition(
//                 line.end_pos.x - to.getRadius(),
//                 line.end_pos.y - to.getRadius()
//             );
//
//             line.draw(*this->window);
//             this->window->draw(from);
//             this->window->draw(to);
//         }
//     }
// }

App::App(
    Engine* engine1,
    Engine* engine2,
    chess::Color playing_color,
    bool player_playing = true,
    bool scorewindow = false
) {
    this->resolution = sf::VideoMode(800, 800);
    this->cellsize = this->resolution.width / 8.0;
    this->window = new sf::RenderWindow(this->resolution, "Chess - Game", sf::Style::Titlebar | sf::Style::Close);
    if (scorewindow)
        this->scorewindow = new sf::RenderWindow(sf::VideoMode(200, 200), "Chess - Score", sf::Style::Titlebar | sf::Style::Close);

    this->board = chess::Board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

    this->enginew = engine1;
    this->enginew->setBoard(&this->board);

    if (player_playing) {
        this->playing_color = playing_color;
        this->engine_color = !playing_color;
        std::cout << "Engine Color: " << this->engine_color << "\n";
    } else {
        this->engineb = engine2;
        this->engineb->setBoard(&this->board);
    }

    for (int i = 0; i <= 18; i++)
        this->piece_textures.push_back(sf::Texture());
    this->piece_textures[0].loadFromFile("gfx/bR.png");
    this->piece_textures[1].loadFromFile("gfx/bKn.png");
    this->piece_textures[2].loadFromFile("gfx/bB.png");
    this->piece_textures[3].loadFromFile("gfx/bQ.png");
    this->piece_textures[4].loadFromFile("gfx/bK.png");
    this->piece_textures[5].loadFromFile("gfx/bP.png");
    this->piece_textures[6].loadFromFile("gfx/wR.png");
    this->piece_textures[7].loadFromFile("gfx/wKn.png");
    this->piece_textures[8].loadFromFile("gfx/wB.png");
    this->piece_textures[9].loadFromFile("gfx/wQ.png");
    this->piece_textures[10].loadFromFile("gfx/wK.png");
    this->piece_textures[11].loadFromFile("gfx/wP.png");

    for (int i = 0; i <= 18; i++) {
        this->piece_sprites.push_back(sf::Sprite(this->piece_textures[i]));
        this->piece_sprites[i].setScale(
            this->cellsize / this->piece_textures[i].getSize().x,
            this->cellsize / this->piece_textures[i].getSize().y
        );
    }
}

App::~App() {
    delete this->window;
    delete this->enginew;

    if (this->engineb != nullptr) {
        delete this->engineb;
    }
}
