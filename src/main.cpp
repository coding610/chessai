#include "engine.hpp"
#include "app.hpp"


int main() {
    // Engine* enginew = new Engine(chess::Color::WHITE, true, 5);
    Engine* engineb = new Engine(chess::Color::BLACK, true, 4);

    App app = App(engineb, nullptr, chess::Color::WHITE, true, false);
    app.run();

    return 0;
}
