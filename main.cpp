#include <iostream>
#include <cassert>
#include "hibari.h"

using namespace std;

int main(int argc, char **argv)
{
    AI *ai = nullptr;
    Hibari hibari;

    if (argc != 2)
    {
        cerr<<argv[0]<<" AI"<<endl;
        return -1;
    }

    if (argv[1] == hibari.getName())
        ai = &hibari;
    else
    {
        cerr<<"AI not found"<<endl;
        return -1;
    }

    cout<<ai->getName()<<endl;

    int turn = 0;

    for (int turn=0; ; turn++)
    {
        int time;
        cin>>time;

        int n;
        cin>>n;
        assert(n==SN);
        int cost[SN];
        for (int &c: cost)
            cin>>c;

        State state[2];
        cin>>state[0]>>state[1];

        Move move = ai->think(state, turn, time);

        cout<<(move.skill==-1 ? 2 : 3)<<endl;
        if (move.skill != -1)
        {
            cout<<move.skill;
            if (1<=move.skill && move.skill<=6)
                cout<<" "<<move.x<<move.y;
            if (move.skill==7)
                cout<<" "<<move.id;
            cout<<endl;
        }
        cout<<move.move[0]<<endl;
        cout<<move.move[1]<<endl;
    }
}
