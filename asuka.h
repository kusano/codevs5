#pragma once

#include "ai.h"
#include "xorshift.h"

class Asuka: public AI
{
    Xorshift rand;
public:
    std::string getName() const override { return "Asuka"; }
    Action think(int cost[SN], State state[2], int turn, int time) override;
};
