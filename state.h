#pragma once

#include <vector>
#include <istream>
#include "base.h"

struct State
{
    const static int BUF = 16;

    int point;
    char map[A];
    int ninja[2];
    int dog[A];     //  -1: none
    bool soul[A];
    int skill[SN];

    int dist[A];

    int logNum = 0;
    int logPoint[BUF];
    char logMap[BUF][A];
    int logNinja[BUF][2];
    int logDog[BUF][A];
    bool logSoul[BUF][A];

    void load(std::istream *s);
    bool canMove(int id, int d) const;
    void move(int id, int d);
    void updateDist();
    void moveDog();
    bool checkCapture() const;
    void push();
    void pop();
    std::string dump() const;
};

std::istream &operator>>(std::istream &s, State &state);
