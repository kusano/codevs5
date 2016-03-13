#include "asuka.h"

Action Asuka::think(State state[2], int turn, int time)
{
    Action action;

    int score = -999999;

    for (int d0=0; d0<5; d0++)
    for (int d1=0; d1<5; d1++)
    for (int d2=0; d2<5; d2++)
    for (int d3=0; d3<5; d3++)
    {
        state[0].push();

        if (state[0].canMove(0, d0))
        {
            state[0].move(0, d0);
            if (state[0].canMove(0, d1))
            {
                state[0].move(0, d1);
                if (state[0].canMove(1, d2))
                {
                    state[0].move(1, d2);
                    if (state[0].canMove(1, d3))
                    {
                        state[0].move(1, d3);

                        state[0].updateDistNinja();
                        state[0].moveDog();
                        state[0].updateDistDog();
                        state[0].updateDistSoul();

                        int s = 0;
                        s += state[0].point*10000;
                        s -= state[0].distSoul[state[0].ninja[0]]*100;
                        s -= state[0].distSoul[state[0].ninja[1]]*100;
                        s += rand.rand()%16;
                        if (state[0].checkCapture())
                            s = -99999;

                        if (s>score)
                        {
                            score = s;
                            action.move[0][0] = dirs[d0];
                            action.move[0][1] = dirs[d1];
                            action.move[0][2] = '\0';
                            action.move[1][0] = dirs[d2];
                            action.move[1][1] = dirs[d3];
                            action.move[1][2] = '\0';
                        }
                    }
                }
            }
        }

        state[0].pop();
    }

    return action;
}
