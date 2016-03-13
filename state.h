#pragma once

#include <vector>
#include <istream>
#include "base.h"

typedef unsigned long long Hash;

struct Action
{
    int skill;  //  -1: none
    int pos;
    int id;
    char move[2][4];
};

struct MoveHist
{
    Hash hash;
    bool rock;
    bool soul;
};

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

    int histNum = 0;
    int histPoint[BUF];
    char histMap[BUF][A];
    int histNinja[BUF][2];
    int histDog[BUF][A];
    bool histSoul[BUF][A];

    void load(std::istream *s);
    bool canMove(int id, int d) const;
    void move(int id, int d, MoveHist *hist=nullptr);
    void undo(int id, int d, const MoveHist &hist);
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
