#include <iostream>
#include <cassert>
#include <ctime>
#include "hibari.h"
#include "asuka.h"
#include "yagyu.h"

using namespace std;

int main(int argc, char **argv)
{
    AI *ai = nullptr;
    Hibari hibari;
    Asuka asuka;
    Yagyu yagyu;

    if (argc != 2)
    {
        cerr<<argv[0]<<" AI"<<endl;
        return -1;
    }

    if (false);
    else if (argv[1]==hibari.getName())
        ai = &hibari;
    else if (argv[1]==asuka.getName())
        ai = &asuka;
    else if (argv[1]==yagyu.getName())
        ai = &yagyu;
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

        clock_t start = clock();

        Action action = ai->think(cost, state, turn, time);

        cout<<(action.skill.id==NONE ? 2 : 3)<<endl;
        if (action.skill.id!=NONE)
        {
            cout<<action.skill.id;
            if (STONE_S<=action.skill.id && action.skill.id<=COPY_E)
                cout<<" "<<action.skill.pos/W<<" "<<action.skill.pos%W;
            if (action.skill.id==SLASH)
                cout<<" "<<action.skill.ninja;
            cout<<endl;
        }
        for (char c: string(action.move[0]))
            if (c!='N')
                cout<<c;
        cout<<endl;
        for (char c: string(action.move[1]))
            if (c!='N')
                cout<<c;
        cout<<endl;

        cerr<<double(clock()-start)/CLOCKS_PER_SEC<<endl;
    }
}
