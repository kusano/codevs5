#pragma once

#include <vector>
#include <istream>

const static int W = 17;
const static int H = 14;
const static int SN = 8;

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

