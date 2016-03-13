#include "yagyu.h"
#include <algorithm>
#include <iostream>

using namespace std;

Move Yagyu::think(State state[2], int turn, int time)
{
    State &s = state[0];

    beam.resize(1);

    for (int depth=0; depth<BD; depth++)
    {
        cerr<<"Depth: "<<depth<<endl;

        beamPre = beam;
        beam.resize(0);
        hash.clear();

        for (Node &node: beamPre)
        {
            s.push();

            for (int i=0; i<depth; i++)
            {
                s.move(0, node.move[i].move[0]);
                s.move(1, node.move[i].move[1]);
                s.updateDistNinja();
                s.moveDog();
            }

            for (int d00=0; d00<5; d00++) if (s.canMove(0, d00)) { s.push(), s.move(0, d00);
            for (int d01=0; d01<5; d01++) if (s.canMove(0, d01)) { s.push(), s.move(0, d01);
            for (int d10=0; d10<5; d10++) if (s.canMove(1, d10)) { s.push(), s.move(1, d10);
            for (int d11=0; d11<5; d11++) if (s.canMove(1, d11)) { s.push(), s.move(1, d11);
            if (hash.count(s.hash)==0)
            {
                hash.insert(s.hash);

                s.updateDistNinja();
                s.updateDistSoul();
                s.moveDog();

                beam.push_back(Node());
                Node &n = beam.back();

                n.score = score(s);
                for (int i=0; i<depth; i++)
                    n.move[i] = node.move[i];
                n.move[depth].skill = -1;
                n.move[depth].move[0][0] = dirs[d00];
                n.move[depth].move[0][1] = dirs[d01];
                n.move[depth].move[0][2] = '\0';
                n.move[depth].move[1][0] = dirs[d10];
                n.move[depth].move[1][1] = dirs[d11];
                n.move[depth].move[1][2] = '\0';
            }
            s.pop(); }
            s.pop(); }
            s.pop(); }
            s.pop(); }

            s.pop();
        }

        sort(beam.begin(), beam.end());
        beam.resize(BW);
    }

    cerr<<"Score: "<<beam[0].score<<endl;

    return beam[0].move[0];
}

bool Yagyu::Node::operator<(const Node &node) const
{
    return score > node.score;
}

long long Yagyu::score(const State &state)
{
    if (state.checkCapture())
        return -99999999LL;

    long long score = 0LL;
    score += 10000LL * state.point;
    score -= 100LL * state.distSoul[state.ninja[0]];
    score -= 100LL * state.distSoul[state.ninja[1]];
    score += rand.rand()%32 - 16;

    return score;
}
