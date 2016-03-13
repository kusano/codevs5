#include "yagyu.h"
#include <algorithm>
#include <iostream>

using namespace std;

Action Yagyu::think(State state[2], int turn, int time)
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
                s.move(0, node.action[i].move[0]);
                s.move(1, node.action[i].move[1]);
                s.updateDistNinja();
                s.moveDog();
            }

            MoveHist hist[6];

            for (int d00=0; d00<5; d00++) if (s.canMove(0, d00)) { s.move(0, d00, &hist[0]);
            for (int d01=0; d01<5; d01++) if (s.canMove(0, d01)) { s.move(0, d01, &hist[1]);
            for (int d10=0; d10<5; d10++) if (s.canMove(1, d10)) { s.move(1, d10, &hist[3]);
            for (int d11=0; d11<5; d11++) if (s.canMove(1, d11)) { s.move(1, d11, &hist[4]);
            {
                if (hash.count(s.hash)>0)
                    goto end1;
                hash.insert(s.hash);

                s.push();

                s.updateDistNinja();
                s.updateDistSoul();
                s.moveDog();

                if (s.checkCapture())
                    goto end2;

                beam.push_back(Node());
                Node &n = beam.back();

                n.score = score(s);
                for (int i=0; i<depth; i++)
                    n.action[i] = node.action[i];
                n.action[depth].skill = -1;
                n.action[depth].move[0][0] = dirs[d00];
                n.action[depth].move[0][1] = dirs[d01];
                n.action[depth].move[0][2] = '\0';
                n.action[depth].move[1][0] = dirs[d10];
                n.action[depth].move[1][1] = dirs[d11];
                n.action[depth].move[1][2] = '\0';

                end2:
                s.pop();

                end1:;
            }
            s.undo(1, d11, hist[4]); }
            s.undo(1, d10, hist[3]); }
            s.undo(0, d01, hist[1]); }
            s.undo(0, d00, hist[0]); }

            s.pop();
        }

        //  ‚±‚Ì[‚³‚Ü‚Å¶‚«Žc‚ê‚È‚¢‚È‚ç‚ÎA’¼‘O‚ÌÅ‘PŽè‚ð‰ð‚Æ‚·‚é
        if (beam.empty())
        {
            beam.push_back(Node());
            beam[0].score = -1;

            if (depth==0)
            {
                beam[0].action[0].skill = -1;
                beam[0].action[0].move[0][0] = '\0';
                beam[0].action[0].move[1][0] = '\0';
            }
            else
                beam[0].action[0] = beamPre[0].action[0];

            break;
        }

        sort(beam.begin(), beam.end());

        if (int(beam.size())>BW)
            beam.resize(BW);
    }

    cerr<<"Score: "<<beam[0].score<<endl;

    return beam[0].action[0];
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
