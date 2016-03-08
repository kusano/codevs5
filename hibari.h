#pragma once

#include "ai.h"
#include "xorshift.h"

class Hibari: AI
{
    Xorshift rand;
public:
    std::string getName() const override { return "Hibari"; }
    Move think(State state[2], int turn, int time) override;
};
