#include <vector>
#include <istream>

const static int W = 17;
const static int H = 14;
const static int SN = 8;

struct Pos
{
    int x, y;

    Pos() {}
    Pos(int x, int y): x(x), y(y) {}
};

struct State
{
    int point;
    char map[H][W];
    Pos ninja[2];
    std::vector<Pos> dog;
    std::vector<Pos> soul;
    int skill[SN];

    void load(std::istream *s);
};

std::istream &operator>>(std::istream &s, State &state);
