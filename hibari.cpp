#include "hibari.h"

Action Hibari::think(int cost[SN], State state[2], int turn, int time)
{
    Action action;

    for (int i=0; i<2; i++)
    {
        for (int j=0; j<2; j++)
            action.move[i][j] = "URLDN"[rand.rand()%5];
        action.move[i][2] = '\0';
    }

    return action;
}
