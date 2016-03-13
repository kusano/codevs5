#include "hibari.h"

Action Hibari::think(State state[2], int turn, int time)
{
    Action action;

    action.skill = -1;

    for (int i=0; i<2; i++)
    {
        for (int j=0; j<2; j++)
            action.move[i][j] = "URLDN"[rand.rand()%5];
        action.move[i][2] = '\0';
    }

    return action;
}
