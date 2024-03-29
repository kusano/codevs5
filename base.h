#pragma once

const static int W = 14;
const static int H = 17;
const static int A = W*H;
const static int SN = 8;

const static int dir[5] = {-W, -1, +1, +W,  0};
const static char *dirs =  "U" "L" "R" "D" "N";

const static int dir8[8] =
{
    -W-1, -W, -W+1,
      -1,       +1,
     W-1,  W,  W+1,
};

const static int INF = 999;
