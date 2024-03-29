#pragma once

#include "ai.h"
#include "xorshift.h"
#include <vector>
#include <unordered_set>

class Yagyu: public AI
{
public:
    std::string getName() const override { return "Yagyu"; }
    Action think(int cost[SN], State state[2], int turn, int time) override;

private:
    const static int BW = 1024;
    const static int BD = 8;

    struct Node
    {
        long long score;
        Action action[BD];

        bool operator<(const Node &node) const;
    };

    Xorshift rand;
    std::vector<Node> beam;
    std::vector<Node> beamPre;
    std::unordered_set<Hash> hash;
    std::vector<Skill> skillCand;

    long long score(const State &state, int dogDist);
};
