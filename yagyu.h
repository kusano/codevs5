#pragma once

#include "ai.h"

class Yagyu: public AI
{
public:
    std::string getName() const override { return "Yagyu"; }
    Move think(State state[2], int turn, int time) override;
};
