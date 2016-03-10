#include "state.h"
#include <cassert>
#include <functional>
#include <sstream>
#include <iomanip>

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

string State::dump() const
{
    stringstream ss;

    ss<<point<<endl;

    char m[A];
    for (int i=0; i<A; i++)
        m[i] = ' ';
    //  wall
    for (int i=0; i<A; i++)
        if (map[i]=='W')
            m[i] = '#';
    //  soul
    for (int s: soul)
        m[s] = '.';
    //  rock
    for (int i=0; i<A; i++)
        if (map[i]=='O')
            m[i] = 'O';
    //  dog
    for (int d: dog)
        m[d] = '*';
    //  ninja
    m[ninja[0]] = m[ninja[1]] = '@';

    for (int y=0; y<H; y++)
    {
        for (int x=0; x<W; x++)
            ss<<m[y*W+x];
        ss<<endl;
    }

    for (int s: skill)
        ss<<" "<<s;
    ss<<endl;

    for (int y=0; y<H; y++)
    {
        for (int x=0; x<W; x++)
            ss<<setw(4)<<dist[y*W+x];
        ss<<endl;
    }

    return ss.str();
}

istream &operator>>(istream &s, State &state)
{
    state.load(&s);
    return s;
}
