#pragma once

#include "state.h"
#include <string>

struct Action
{
    int skill;  //  -1: none
    int pos;
    int id;
    char move[2][4];
};

class AI
{
public:
    virtual std::string getName() const = 0;
    virtual Action think(State state[2], int turn, int time) = 0;
};
