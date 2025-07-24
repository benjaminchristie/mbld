#include "cube.h"
#include <stdio.h>

/*
Note that
corners:
0,1,2,3 are the top layer
4,5,6,7 are the bot layer
edges:
0,1,2,3 are the top layer
4,5,6,7 are the mid slice
8,9,10,11 are thebo layer
*/
void applyMoves333(Cube333 *cube, enum Moves333 m) {
  Corner* corners = cube->corners;
  switch (m) {
  case L: // c: 0->4->7->3, e: 3->7->11->6
    uint32_t c0 = corners[0];
    uint32_t c4 = corners[4];
    uint32_t c7 = corners[7];
    uint32_t c3 = corners[3];
    break;
  default:;
  }
  return;
}

Cube333 *newCube(void) {
  // starting orientation is white top green front
  Cube333 *cube;
  if ((cube = malloc(sizeof(Cube333))) == NULL) {
    return NULL;
  }

  /* clang-format off */

  cube->corners[0] = 0b00110001; // wog
  cube->corners[1] = 0b00111000; // wgr
  cube->corners[2] = 0b00101010; // wrb
  cube->corners[3] = 0b00100011; // wbo
  cube->corners[4] = 0b00010101; // yog
  cube->corners[5] = 0b00011100; // ygr
  cube->corners[6] = 0b00001110; // yrb
  cube->corners[7] = 0b00000111; // ybo


  cube->edges[0]   = 0b00110000;  // wg
  cube->edges[1]   = 0b00101000;  // wr
  cube->edges[2]   = 0b00100010;  // wb
  cube->edges[3]   = 0b00100001;  // wo
  cube->edges[4]   = 0b00011000;  // gr
  cube->edges[5]   = 0b00010001;  // go
  cube->edges[6]   = 0b00001010;  // rb
  cube->edges[7]   = 0b00000011;  // ob
  cube->edges[8]   = 0b00010100;  // gy
  cube->edges[9]   = 0b00001100;  // ry
  cube->edges[10]  = 0b00000110;  // by
  cube->edges[11]  = 0b00000101;  // oy

  /* clang-format on */

  return cube;
}

void freeCube(Cube333 *c) { free(c); }

int main(int argc, char **argv) {
  Cube333 *c = newCube();
  for (int i = 0; i < 12; i++) {
    printf("%d\n", c->edges[i]);
  }
  for (int i = 0; i < 8; i++) {
    printf("%d\n", c->corners[i]);
  }
  freeCube(c);
  return 0;
}
