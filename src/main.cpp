#include "engine/enginev1.hpp"
#include "app.hpp"


int main() {
    EngineV1* enginew = new EngineV1(chess::Color::WHITE, 6);
    EngineV1* engineb = new EngineV1(chess::Color::BLACK, 6);

    App app = App(enginew, engineb, chess::Color::WHITE, true, false);
    app.run();

    return 0;
}
