#pragma once

class Xorshift
{
    unsigned int x;
public:
    Xorshift(int seed=2463534242);
    int rand();
};
