#pragma once

#include <vector>
#include <istream>
#include "base.h"

struct State
{
    int point;
    char map[A];
    int ninja[2];
    int dog[A];     //  -1: none
    bool soul[A];
    int skill[SN];

    int dist[A];

    void load(std::istream *s);
    void updateDist();
    std::string dump() const;
};

std::istream &operator>>(std::istream &s, State &state);
