#include <iostream>
#include <cassert>
#include "yagyu.h"

using namespace std;

int main(int argc, char **argv)
{
    Yagyu yagyu;
    AI *ai = &yagyu;

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
    }
}
