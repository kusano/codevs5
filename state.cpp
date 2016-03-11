#include "state.h"
#include <cassert>
#include <functional>
#include <sstream>
#include <iomanip>
#include <utility>
#include <algorithm>

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

    for (int i=0; i<A; i++)
        dog[i] = -1;
    *s>>n;
    for (int i=0; i<n; i++)
    {
        int id, x, y;
        *s>>id>>y>>x;;
        dog[y*W+x] = id;
    }

    for (int i=0; i<A; i++)
        soul[i] = false;
    *s>>n;
    for (int i=0; i<n; i++)
    {
        int x, y;
        *s>>y>>x;
        soul[y*W+x] = true;
    }

    for (int &sk: skill)
        *s>>sk;
}

bool State::canMove(int id, int d) const
{
    int p1 = ninja[id]+d;
    int p2 = ninja[id]+d+d;
    return map[p1]=='_' ||
        map[p1]=='O' &&
        map[p2]=='_' &&
        dog[p2]==-1 &&
        p2!=ninja[1-id];
}

void State::move(int id, int d)
{
    int p = ninja[id];

    if (map[p+d]=='O')
    {
        map[p+d+d] = 'O';
        map[p+d] = '_';
    }

    point += int(soul[p+d])*2;
    soul[p+d] = false;

    ninja[id] += d;
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

void State::moveDog()
{
    pair<pair<int,int>,int> tmp[A];
    int n = 0;

    for (int i=0; i<A; i++)
        if (dog[i]>=0)
            tmp[n++] = make_pair(make_pair(dist[i], dog[i]), i);
    sort(tmp, tmp+n);

    for (int i=0; i<n; i++)
    {
        int p = tmp[i].second;
        for (int d=0; d<4; d++)
        {
            int t = p+dir[d];
            if (map[t]=='_' &&
                dog[t]==-1 &&
                dist[t]==dist[p]-1)
            {
                dog[t] = dog[p];
                dog[p] = -1;
                break;
            }
        }
    }
}

bool State::checkCapture() const
{
    return dog[ninja[0]]>=0 || dog[ninja[1]]>=0;
}

void State::push()
{
    assert(logNum<BUF);

    logPoint[logNum] = point;
    memcpy(logMap[logNum], map, sizeof map);
    logNinja[logNum][0] = ninja[0];
    logNinja[logNum][1] = ninja[1];
    memcpy(logDog[logNum], dog, sizeof dog);
    memcpy(logSoul[logNum], soul, sizeof soul);

    logNum++;
}

void State::pop()
{
    logNum--;

    assert(logNum>=0);

    point = logPoint[logNum];
    memcpy(map, logMap[logNum], sizeof map);
    ninja[0] = logNinja[logNum][0];
    ninja[1] = logNinja[logNum][1];
    memcpy(dog, logDog[logNum], sizeof dog);
    memcpy(soul, logSoul[logNum], sizeof soul);
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
    for (int i=0; i<A; i++)
        if (soul[i])
            m[i] = '.';
    //  rock
    for (int i=0; i<A; i++)
        if (map[i]=='O')
            m[i] = 'O';
    //  dog
    for (int i=0; i<A; i++)
        if (dog[i]>=0)
            m[i] = '*';
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
