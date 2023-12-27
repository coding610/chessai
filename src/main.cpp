#include "app.hpp"
#include "chess.hpp"
#include "engine.hpp"


int main() {
    Engine* engine1 = new Engine(chess::Color::WHITE, true, 6);
    Engine* engine2 = new Engine(chess::Color::BLACK, true, 6);

    App app(engine1, engine2, chess::Color::NONE, false);
    app.run();
}
