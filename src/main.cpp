#include "app.hpp"
#include "chess.hpp"
#include "engine.hpp"


int main() {
    Engine* enginew = new Engine(chess::Color::WHITE, true, 2 * 3);
    Engine* engineb = new Engine(chess::Color::BLACK, true, 6);

    App app(enginew, engineb, chess::Color::NONE, false);
    app.run();
}
