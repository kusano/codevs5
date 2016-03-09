#pragma once

#include <vector>
#include <istream>
#include "base.h"

struct State
{
    int point;
    char map[W*H];
    int ninja[2];
    std::vector<int> dog;
    std::vector<int> soul;
    int skill[SN];

    void load(std::istream *s);
};

std::istream &operator>>(std::istream &s, State &state);

