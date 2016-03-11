#pragma once

#include "ai.h"
#include "xorshift.h"

class Asuka: public AI
{
    Xorshift rand;
public:
    std::string getName() const override { return "Asuka"; }
    Move think(State state[2], int turn, int time) override;
};
