#include "hibari.h"

Move Hibari::think(State state[2], int turn, int time)
{
    Move move;

    move.skill = -1;

    for (int i=0; i<2; i++)
    {
        for (int j=0; j<2; j++)
            move.move[i][j] = "URLDN"[rand.rand()%5];
        move.move[i][2] = '\0';
    }

    return move;
}
