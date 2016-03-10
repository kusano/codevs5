#include "state.h"
#include <cassert>

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

istream &operator>>(istream &s, State &state)
{
    state.load(&s);
    return s;
}
