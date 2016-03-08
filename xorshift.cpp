#include "xorshift.h"

Xorshift::Xorshift(int seed/*=2463534242*/)
{
    x = static_cast<unsigned int>(seed);
}

int Xorshift::rand()
{
    x ^= x<<13;
    x ^= x>>17;
    x ^= x<< 5;
    return static_cast<int>(x>>1);
}
