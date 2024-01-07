#pragma once

#include "chess.hpp"

class Engine {
public:
    virtual void set_board(chess::Board* b) = 0;
    virtual chess::Move think() = 0;
};
