#include "state.h"
#include <cassert>

using namespace std;

void State::load(istream *s)
{
    *s>>point;

    int w, h;
    *s>>w>>h;
    assert(w==W && h==H);

    for (int y=0; y<H; y++)
    for (int x=0; x<W; x++)
        *s>>map[y][x];

    int n;
    *s>>n;
    assert(n==2);
    for (Pos &n: ninja)
    {
        int id, x, y;
        *s>>id>>y>>x;
        n = Pos(x, y);
    }

    *s>>n;
    dog.resize(n);
    for (Pos &d: dog)
    {
        int id, x, y;
        *s>>id>>y>>x;;
        d = Pos(x, y);
    }

    *s>>n;
    soul.resize(n);
    for (Pos &sl: soul)
        *s>>sl.y>>sl.x;

    for (int &sk: skill)
        *s>>sk;
}

istream &operator>>(istream &s, State &state)
{
    state.load(&s);
    return s;
}
