#pragma once

#include <vector>
#include <istream>
#include "base.h"

struct State
{
    int point;
    char map[A];
    int ninja[2];
    std::vector<int> dog;
    std::vector<int> soul;
    int skill[SN];

    int dist[A];

    void load(std::istream *s);
    void updateDist();
};

std::istream &operator>>(std::istream &s, State &state);
