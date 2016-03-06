#include <iostream>
#include <cassert>
#include "state.h"

using namespace std;

int main()
{
    cout<<"hogehoge"<<endl;
    while (true)
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

        cout<<"2"<<endl;
        cout<<endl;
        cout<<endl;
    }
}

