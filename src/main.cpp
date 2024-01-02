#include "engine.hpp"
#include "app.hpp"


int main() {
    Engine* enginew = new Engine(chess::Color::WHITE, true, 3);
    Engine* engineb = new Engine(chess::Color::BLACK, true, 3);

    App app = App(engineb, nullptr, chess::Color::WHITE, true, false);
    app.run();

    return 0;
}
