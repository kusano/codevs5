#include "yagyu.h"
#include <algorithm>
#include <iostream>

using namespace std;

Action Yagyu::think(int cost[SN], State state[2], int turn, int time)
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

            int dogDist;

            for (int i=0; i<depth; i++)
            {
                s.move(0, node.action[i].move[0]);
                s.move(1, node.action[i].move[1]);
                s.updateDistNinja();
                s.moveDog();

                //  [‚³0‚Å‚Ì”EŒ¢‚Æ‚Ì‹——£‚ðŠo‚¦‚Ä‚¨‚­
                if (i==0)
                {
                    s.updateDistDog();
                    dogDist = min(
                        s.distDog[s.ninja[0]],
                        s.distDog[s.ninja[1]]);
                }
            }

            skillCand.resize(1);
            skillCand[0] = Skill();

            //  ”Ep‚Í[‚³0‚Å‚Ì‚ÝŽg—p‚·‚é
            if (depth==0)
            {
                //  ’´‚‘¬
                if (s.point>=cost[ACCEL])
                {
                    skillCand.push_back(Skill(ACCEL, 0, 0, cost[ACCEL]));
                }

                //  Ž©•ªg
                if (s.point>=cost[COPY_S])
                {
                    int n = 0;
                    for (int i=0; i<1000 && n<8; i++)
                    {
                        int p = rand.rand()%A;
                        if (s.map[p]=='_')
                        {
                            skillCand.push_back(Skill(COPY_S, p, 0, cost[COPY_S]));
                            n++;
                        }
                    }
                }

                //  ‰ñ“]Ža
                if (s.point>=cost[SLASH])
                {
                    skillCand.push_back(Skill(SLASH, 0, 0, cost[SLASH]));
                    skillCand.push_back(Skill(SLASH, 0, 1, cost[SLASH]));
                }
            }

            HistSpell histSpell;
            HistMove histMove[6];

            for (const Skill &skill: skillCand) { s.spell(skill, &histSpell);
            for (int d00=0; d00<5; d00++) if (s.canMove(0, d00)) { s.move(0, d00, &histMove[0]);
            for (int d01=0; d01<5; d01++) if (s.canMove(0, d01)) { s.move(0, d01, &histMove[1]);
            for (int d02=(skill.id==ACCEL?0:4); d02<5; d02++)
                                          if (s.canMove(0, d02)) { s.move(0, d02, &histMove[2]);
            for (int d10=0; d10<5; d10++) if (s.canMove(1, d10)) { s.move(1, d10, &histMove[3]);
            for (int d11=0; d11<5; d11++) if (s.canMove(1, d11)) { s.move(1, d11, &histMove[4]);
            for (int d12=(skill.id==ACCEL?0:4); d12<5; d12++)
                                          if (s.canMove(1, d12)) { s.move(1, d12, &histMove[5]);
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

                //  ”EŒ¢‚Æ‚Ì‹——£‚ðƒRƒXƒg‚ÉŽg—p‚·‚é
                if (depth==0)
                {
                    s.updateDistDog();
                    dogDist = min(
                        s.distDog[s.ninja[0]],
                        s.distDog[s.ninja[1]]);
                }

                beam.push_back(Node());
                Node &n = beam.back();

                n.score = score(s, dogDist);
                for (int i=0; i<depth; i++)
                    n.action[i] = node.action[i];
                n.action[depth].skill = skill;
                n.action[depth].move[0][0] = dirs[d00];
                n.action[depth].move[0][1] = dirs[d01];
                n.action[depth].move[0][2] = dirs[d02];
                n.action[depth].move[0][3] = '\0';
                n.action[depth].move[1][0] = dirs[d10];
                n.action[depth].move[1][1] = dirs[d11];
                n.action[depth].move[1][2] = dirs[d12];
                n.action[depth].move[1][3] = '\0';

                end2:
                s.pop();

                end1:;
            }
            s.undoMove(1, d12, histMove[5]); }
            s.undoMove(1, d11, histMove[4]); }
            s.undoMove(1, d10, histMove[3]); }
            s.undoMove(0, d02, histMove[2]); }
            s.undoMove(0, d01, histMove[1]); }
            s.undoMove(0, d00, histMove[0]); }
            s.undoSpell(skill, histSpell); }

            s.pop();
        }

        //  ‚±‚Ì[‚³‚Ü‚Å¶‚«Žc‚ê‚È‚¢‚È‚ç‚ÎA’¼‘O‚ÌÅ‘PŽè‚ð‰ð‚Æ‚·‚é
        if (beam.empty())
        {
            beam.push_back(Node());
            beam[0].score = -1;

            if (depth==0)
            {
                beam[0].action[0] = Action();
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

long long Yagyu::score(const State &state, int dogDist)
{
    if (state.checkCapture())
        return -99999999LL;

    long long score = 0LL;
    score += 10000LL * state.point;
    score -= 100LL * state.distSoul[state.ninja[0]];
    score -= 100LL * state.distSoul[state.ninja[1]];
    score += rand.rand()%32 - 16;

    if (dogDist==1)
        score -= 10000LL;

    return score;
}
