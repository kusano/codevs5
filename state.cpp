#include "state.h"
#include <cassert>
#include <functional>

using namespace std;

void State::load(istream *s)
{
    *s>>point;

    int w, h;
    *s>>h>>w;
    assert(w==W && h==H);

    for (int i=0; i<A; i++)
        *s>>map[i];

    int n;
    *s>>n;
    assert(n==2);
    for (int &n: ninja)
    {
        int id, x, y;
        *s>>id>>y>>x;
        n = y*W+x;
    }

    *s>>n;
    dog.resize(n);
    for (int &d: dog)
    {
        int id, x, y;
        *s>>id>>y>>x;;
        d = y*W+x;
    }

    *s>>n;
    soul.resize(n);
    for (int &sl: soul)
    {
        int x, y;
        *s>>y>>x;
        sl = y*W+x;
    }

    for (int &sk: skill)
        *s>>sk;
}

void State::updateDist()
{
    for (int i=0; i<A; i++)
        dist[i] = INF;

    function<void (int, int)> BT = [&](int p, int d)
    {
        if (dist[p]<=d)
            return;

        dist[p] = d;
        for (int r=0; r<4; r++)
            if (map[p+dir[r]] == '_')
                BT(p+dir[r], d+1);
    };

    BT(ninja[0], 0);
    BT(ninja[1], 0);
}

istream &operator>>(istream &s, State &state)
{
    state.load(&s);
    return s;
}
