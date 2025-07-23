#pragma once

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

enum Color { Red, Green, White };

typedef struct {
  char *edges;
  char *corners;
} Cube;

typedef struct {
  Cube **data;
  uint32_t n;
} Cubes;

void printColChar(char c, enum Color col);
void clearScreen(void);
void handleInput(char c);

Cube *_scramble_cube(void);
void _free_cube(Cube* c);
Cubes *scramble(uint32_t n);
void freeCubes(Cubes *ptr);


void displayCube(Cubes* c, uint32_t i);

bool game(uint32_t n);
