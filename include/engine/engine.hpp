#pragma once

#include "chess.hpp"

class Engine {
public:
    virtual void setBoard(chess::Board* b) = 0;
    virtual chess::Move think() = 0;
};
