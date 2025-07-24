#pragma once

#include <stdint.h>
#include <stdlib.h>

/*
defining flags as follows:
0b00000000
  abcdefgh
a: unused
b: solved
c: white
d: green
e: red
f: yellow
g: blue
h: orange

note that edge must have two flags set, corner has three, and center has 1
*/
typedef uint8_t Edge;
typedef uint8_t Corner;
typedef uint8_t Center;

enum Moves333 {
  L,
  Lp,
  L2,
  R,
  Rp,
  R2,
  F,
  Fp,
  F2,
  B,
  Bp,
  B2,
  U,
  Up,
  U2,
  D,
  Dp,
  D2,
  M,
  Mp,
  M2,
  E,
  Ep,
  E2,
  S,
  Sp,
  S2,
  x,
  xp,
  x2,
  y,
  yp,
  y2,
  z,
  zp,
  z2
};

typedef struct {
  Edge edges[12];
  Corner corners[8];
  Center centers[6];
} Cube333;

Cube333 *newCube(void);
void freeCube(Cube333 *c);
