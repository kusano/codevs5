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

    copy = -1;
    dogNum = 0;

    updateHash();
}

bool State::canMove(int id, int d) const
{
    int p1 = ninja[id]+dir[d];
    int p2 = ninja[id]+dir[d]*2;
    return map[p1]=='_' ||
        map[p1]=='O' &&
        map[p2]=='_' &&
        dog[p2]==-1 &&
        p2!=ninja[1-id];
}

void State::move(int id, int d, HistMove *hist/*=nullptr*/)
{
    int p = ninja[id];
    int t = dir[d];

    if (hist)
    {
        hist->hash = hash;
        hist->rock = map[p+t]=='O';
        hist->soul = soul[p+t];
    }

    if (map[p+t]=='O')
    {
        map[p+t+t] = 'O';
        map[p+t] = '_';

        hash ^= hashRock[p+t+t];
        hash ^= hashRock[p+t];
    }

    if (soul[p+t])
    {
        hash ^= hashPoint[point%64];
        point += 2;
        hash ^= hashPoint[point%64];

        soul[p+t] = false;
        hash ^= hashSoul[p+t];

        dogNum--;
    }

    ninja[id] += t;

    hash ^= hashNinja[id][p];
    hash ^= hashNinja[id][p+t];
}

void State::move(int id, const char *m)
{
    for (int i=0; m[i]!='\0'; i++)
        for (int d=0; d<4; d++)
            if (dirs[d]==m[i])
                move(id, d);
}

void State::undoMove(int id, int d, const HistMove &hist)
{
    int p = ninja[id];
    int t = dir[d];

    hash = hist.hash;

    if (hist.rock)
    {
        map[p+t] = '_';
        map[p] = 'O';
    }

    if (hist.soul)
    {
        point -= 2;
        soul[p] = true;

        dogNum++;
    }

    ninja[id] -= t;
}

void State::spell(const Skill &skill, HistSpell *hist)
{
    hist->hash = hash;

    hash ^= hashPoint[point%64];
    point -= skill.cost;
    hash ^= hashPoint[point%64];

    int p;

    switch (skill.id)
    {
    case NONE:
        break;
    case ACCEL:
        //  処理は動き生成部で行う
        break;
    case STONE_S:
        map[skill.pos] = 'O';
        hash ^= hashRock[skill.pos];
        break;
    case THUNDER_S:
        map[skill.pos] = '_';
        hash ^= hashRock[skill.pos];
        break;
    case COPY_S:
        copy = skill.pos;
        hash ^= hashCopy[copy];
        break;
    case SLASH:
        p = ninja[skill.ninja];
        for (int d=0; d<8; d++)
        {
            int t = p+dir8[d];
            hist->dogId[d] = dog[t];
            if (dog[t]>=0)
                dogNum -= 2,
                hash ^= hashDog[t];
            dog[t] = -1;
        }
        break;
    default:
        assert(false);
    }
}

void State::undoSpell(const Skill &skill, const HistSpell &hist)
{
    hash = hist.hash;

    point += skill.cost;

    int p;

    switch (skill.id)
    {
    case NONE:
        break;
    case ACCEL:
        break;
    case STONE_S:
        map[skill.pos] = '_';
        break;
    case THUNDER_S:
        map[skill.pos] = 'O';
        break;
    case COPY_S:
        copy = -1;
        break;
    case SLASH:
        p = ninja[skill.ninja];
        for (int d=0; d<8; d++)
            if (hist.dogId[d]>=0)
                dog[p+dir8[d]] = hist.dogId[d],
                dogNum += 2;
        break;
    default:
        assert(false);
    }
}

void State::updateDistNinja()
{
    for (int i=0; i<A; i++)
        distNinja[i] = INF;

    int v[A];
    int p1 = 0;
    int p2 = 0;

    if (copy==-1)
    {
        v[p2++] = ninja[0],
        distNinja[ninja[0]] = 0;
        if (ninja[0]!=ninja[1])
            v[p2++] = ninja[1],
            distNinja[ninja[1]] = 0;
    }
    else
    {
        v[p2++] = copy;
        distNinja[copy] = 0;
    }

    bool f[A];
    for (int i=0; i<A; i++)
        f[i] = map[i]=='_' && distNinja[i]==INF;

    while (p1<p2)
    {
        int p = v[p1++];
        for (int d=0; d<4; d++)
        {
            int t = p+dir[d];
            if (f[t])
                f[t] = false,
                distNinja[t] = distNinja[p]+1,
                v[p2++] = t;
        }
    }
}

void State::updateDistDog()
{
    for (int i=0; i<A; i++)
        distDog[i] = INF;

    int v[A];
    int p1 = 0;
    int p2 = 0;

    for (int i=0; i<A; i++)
        if (dog[i]>=0)
            distDog[i] = 0,
            v[p2++] = i;

    while (p1<p2)
    {
        int p = v[p1++];
        for (int d=0; d<4; d++)
        {
            int t = p+dir[d];
            if (map[t]=='_' &&
                distDog[t]==INF)
                distDog[t] = distDog[p]+1,
                v[p2++] = t;
        }
    }
}

void State::updateDistSoul()
{
    for (int i=0; i<A; i++)
        distSoul[i] = INF;

    int v[A];
    int p1 = 0;
    int p2 = 0;

    for (int i=0; i<A; i++)
        if (soul[i] && map[i]=='_')
            distSoul[i] = 0,
            v[p2++] = i;

    bool f[A];
    for (int i=0; i<A; i++)
        f[i] = map[i]=='_' && dog[i]==-1 && distSoul[i]==INF;

    while (p1<p2)
    {
        int p = v[p1++];
        for (int d=0; d<4; d++)
        {
            int t = p+dir[d];
            if (f[t])
                f[t] = false,
                distSoul[t] = distSoul[p]+1,
                v[p2++] = t;
        }
    }
}

void State::moveDog()
{
    pair<pair<int,int>,int> tmp[A];
    int n = 0;

    for (int i=0; i<A; i++)
        if (dog[i]>=0)
            tmp[n++] = make_pair(make_pair(distNinja[i], dog[i]), i);
    sort(tmp, tmp+n);

    for (int i=0; i<n; i++)
    {
        int p = tmp[i].second;
        for (int d=0; d<4; d++)
        {
            int t = p+dir[d];
            if (map[t]=='_' &&
                dog[t]==-1 &&
                distNinja[t]==distNinja[p]-1)
            {
                dog[t] = dog[p];
                dog[p] = -1;

                hash ^= hashDog[t];
                hash ^= hashDog[p];

                break;
            }
        }
    }

    //  犬の移動が完了したら分身は消滅
    if (copy>=0)
    {
        hash ^= hashCopy[copy];
        copy = -1;
    }
}

bool State::checkCapture() const
{
    return dog[ninja[0]]>=0 || dog[ninja[1]]>=0;
}

void State::push()
{
    assert(histNum<BUF);

    histPoint[histNum] = point;
    memcpy(histMap[histNum], map, sizeof map);
    histNinja[histNum][0] = ninja[0];
    histNinja[histNum][1] = ninja[1];
    memcpy(histDog[histNum], dog, sizeof dog);
    memcpy(histSoul[histNum], soul, sizeof soul);
    histCopy[histNum] = copy;
    histDogNum[histNum] = dogNum;

    histNum++;
}

void State::pop()
{
    histNum--;

    assert(histNum>=0);

    point = histPoint[histNum];
    memcpy(map, histMap[histNum], sizeof map);
    ninja[0] = histNinja[histNum][0];
    ninja[1] = histNinja[histNum][1];
    memcpy(dog, histDog[histNum], sizeof dog);
    memcpy(soul, histSoul[histNum], sizeof soul);
    copy = histCopy[histNum];
    dogNum = histDogNum[histNum];

    updateHash();
}

void State::updateHash()
{
    hash = 0LL;

    for (int i=0; i<A; i++)
    {
        //  岩以外は位置が変化しないので岩のみ考慮する
        if (map[i]=='O')
            hash ^= hashRock[i];
        //  IDは考慮しなくて良いでしょう
        if (dog[i]>=0)
            hash ^= hashDog[i];
        if (soul[i])
            hash ^= hashSoul[i];
    }

    hash ^= hashNinja[0][ninja[0]];
    hash ^= hashNinja[1][ninja[1]];

    hash ^= hashPoint[point%64];

    if (copy>=0)
        hash ^= hashCopy[copy];
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
            ss<<setw(4)<<distNinja[y*W+x];
        ss<<endl;
    }

    for (int y=0; y<H; y++)
    {
        for (int x=0; x<W; x++)
            ss<<setw(4)<<distDog[y*W+x];
        ss<<endl;
    }

    for (int y=0; y<H; y++)
    {
        for (int x=0; x<W; x++)
            ss<<setw(4)<<distSoul[y*W+x];
        ss<<endl;
    }

    return ss.str();
}

istream &operator>>(istream &s, State &state)
{
    state.load(&s);
    return s;
}
