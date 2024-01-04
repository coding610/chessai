#include "engine.hpp"
#include "app.hpp"


int main() {
    // Engine* enginew = new Engine(chess::Color::WHITE, 4);
    Engine* enginew = new Engine(chess::Color::WHITE, 5);
    Engine* engineb = new Engine(chess::Color::BLACK, 5);

    App app = App(enginew, engineb, chess::Color::WHITE, false, false);
    app.run();

    return 0;
}
