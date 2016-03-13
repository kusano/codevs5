#pragma once

#include "state.h"
#include <string>

class AI
{
public:
    virtual std::string getName() const = 0;
    virtual Action think(State state[2], int turn, int time) = 0;
};
