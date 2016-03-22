#include "yagyu.h"
#include <algorithm>
#include <iostream>
#define ACCEL ACCEL_
#include <Windows.h>
#undef ACCEL

#pragma comment(lib, "winmm.lib")

using namespace std;

Action Yagyu::think(int cost[SN], State state[2], int turn, int time)
{
    DWORD start = timeGetTime();

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

                //  深さ0での忍犬との距離を覚えておく
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

            //  忍術は深さ0でのみ使用する
            if (depth==0)
            {
                //  超高速
                if (s.point>=cost[ACCEL])
                    skillCand.push_back(Skill(ACCEL, 0, 0, cost[ACCEL]));

                //  自落石、自雷撃
                //for (int i=0; i<2; i++)
                //for (int y=s.ninja[i]/W-2; y<=s.ninja[i]/W+2; y++)
                //for (int x=s.ninja[i]%W-2; x<=s.ninja[i]%W+2; x++)
                //if (0<=x && x<W && 0<=y && y<H)
                //{
                //    int p = y*W+x;
                //    if (s.point>=cost[STONE_S] &&
                //        s.map[i]=='_' &&
                //        p!=s.ninja[0] && p!=s.ninja[1] &&
                //        s.dog[i]==-1 &&
                //        !s.soul[i])
                //        skillCand.push_back(Skill(STONE_S, p, 0, cost[STONE_S]));
                //    if (s.point>=cost[THUNDER_S] &&
                //        s.map[i]=='O')
                //        skillCand.push_back(Skill(THUNDER_S, p, 0, cost[THUNDER_S]));
                //}

                //  自分身
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

                //  回転斬
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
                //  自分身を使用していない場合、この時点で捕縛されていれば脱出不可
                if (s.checkCapture() && skill.id!=COPY_S)
                    goto end1;

                if (hash.count(s.hash)>0)
                    goto end1;
                hash.insert(s.hash);

                s.push();

                s.updateDistNinja();
                s.updateDistSoul();
                s.moveDog();

                if (s.checkCapture())
                    goto end2;

                //  忍犬との距離をコストに使用する
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

        //  この深さまで生き残れないならば、直前の最善手を解とする
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

        DWORD current = timeGetTime();
        //  長くとも150ターン程度で終わるはず
        if (current-start >= time/(300-turn)*2)
            break;
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
    score +=  5000LL * state.point;
    score -= 10000LL * state.dogNum;
    score -=   100LL * state.distSoul[state.ninja[0]];
    score -=   100LL * state.distSoul[state.ninja[1]];
    score += rand.rand()%32 - 16;

    if (dogDist==1)
        score -= 30000LL;

    return score;
}
