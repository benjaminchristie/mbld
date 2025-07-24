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

typedef struct {
  Cubes* cubes;
  uint16_t flags;
  uint16_t screen_idx;
  struct termios old_settings;
  struct termios new_settings;
} GameState;

bool isGameStateSolving(GameState* gs);
bool isGameStateMemoing(GameState* gs);
bool isGameStateExecuting(GameState* gs);
void setGameStateSolving(GameState* gs);
void setGameStateMemoing(GameState* gs);
void setGameStateExecuting(GameState* gs);

GameState* allocateGameState(Cubes* cubes);
void deallocateGameState(GameState* gs);

void printColChar(char c, enum Color col);
void clearScreen(void);
void handleInput(char c);

Cube *_scramble_cube(void);
void _free_cube(Cube* c);
Cubes *scramble(uint32_t n);
void freeCubes(Cubes *ptr);


void _displayCube(Cubes* c, uint32_t i);
void displayGame(GameState* gs);

bool game(uint32_t n);
