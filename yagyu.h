#pragma once

#include "ai.h"
#include "xorshift.h"
#include <vector>
#include <set>

class Yagyu: public AI
{
public:
    std::string getName() const override { return "Yagyu"; }
    Move think(State state[2], int turn, int time) override;

private:
    const static int BW = 32;
    const static int BD = 4;

    struct Node
    {
        long long score;
        Move move[BD];

        bool operator<(const Node &node) const;
    };

    Xorshift rand;
    std::vector<Node> beam;
    std::vector<Node> beamPre;
    std::set<Hash> hash;

    long long score(const State &state);
};
