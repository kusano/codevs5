#pragma once

#include <vector>
#include <istream>
#include "base.h"

typedef unsigned long long Hash;

enum SkillId
{
    NONE        = -1,   //  ����
    ACCEL       = 0,    //  ������
    STONE_S     = 1,    //  ������
    STONE_E     = 2,    //  �G����
    THUNDER_S   = 3,    //  ������
    THUNDER_E   = 4,    //  �G����
    COPY_S      = 5,    //  �����g
    COPY_E      = 6,    //  �G���g
    SLASH       = 7,    //  ��]�a
};

struct Skill
{
    SkillId id;
    int pos;
    int ninja;
    int cost;

    Skill(SkillId id=NONE, int pos=0, int ninja=0, int cost=0)
        :id(id), pos(pos), ninja(ninja), cost(cost)
    {}
};

struct Action
{
    Skill skill;
    char move[2][4];

    Action() {move[0][0] = move[1][0] = '\0';}
};

struct HistMove
{
    Hash hash;
    bool rock;
    bool soul;
};

struct HistSpell
{
    Hash hash;
    int dogId[8];
};

struct State
{
    const static int BUF = 16;

    const static Hash hashRock[A];
    const static Hash hashNinja[2][A];
    const static Hash hashDog[A];
    const static Hash hashSoul[A];
    const static Hash hashPoint[64];
    const static Hash hashCopy[A];

    int point;
    char map[A];
    int ninja[2];
    int dog[A];     //  -1: none
    bool soul[A];
    int skill[SN];

    int copy;       //  -1: none
    int dogNum;

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
    int histCopy[BUF];
    int histDogNum[BUF];

    void load(std::istream *s);
    bool canMove(int id, int d) const;
    void move(int id, int d, HistMove *hist=nullptr);
    void undoMove(int id, int d, const HistMove &hist);
    void move(int id, const char *m);
    void spell(const Skill &skill, HistSpell *hist);
    void undoSpell(const Skill &skill, const HistSpell &hist);
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
