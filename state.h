#pragma once

#include <vector>
#include <istream>
#include "base.h"

typedef unsigned long long Hash;

struct State
{
    const static int BUF = 16;

    const static Hash hashRock[A];
    const static Hash hashNinja[2][A];
    const static Hash hashDog[A];
    const static Hash hashSoul[A];

    int point;
    char map[A];
    int ninja[2];
    int dog[A];     //  -1: none
    bool soul[A];
    int skill[SN];

    Hash hash;

    int distNinja[A];
    int distDog[A];
    int distSoul[A];

    int logNum = 0;
    int logPoint[BUF];
    char logMap[BUF][A];
    int logNinja[BUF][2];
    int logDog[BUF][A];
    bool logSoul[BUF][A];

    void load(std::istream *s);
    bool canMove(int id, int d) const;
    void move(int id, int d);
    void move(int id, const char *m);
    void updateDistNinja();
    void updateDistDog();
    void updateDistSoul();
    void moveDog();
    bool checkCapture() const;
    void push();
    void pop();
    void updateHash();
    std::string dump() const;
};

std::istream &operator>>(std::istream &s, State &state);
