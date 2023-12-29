#include "app.hpp"
#include "chess.hpp"
#include "engine.hpp"


int main() {
    // Engine* enginew = new Engine(chess::Color::WHITE, true, 2 * 3);
    Engine* engineb = new Engine(chess::Color::BLACK, false, 4);

    App app(engineb, nullptr, chess::Color::WHITE, false);
    app.run();
}
