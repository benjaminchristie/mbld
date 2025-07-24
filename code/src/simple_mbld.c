#include "mbld.h"

#define min_c 4
#define max_c 11
#define min_e 8
#define max_e 15
static const float corner_cdf[max_c - min_c] = {
    0.05, // 4
    0.15, // 5
    0.35, // 6
    0.70, // 7
    0.85, // 8
    0.95, // 9
    1.00  // 10
};
static const float edge_cdf[max_e - min_e] = {
    0.10, // 8
    0.25, // 9
    0.45, // 10
    0.85, // 11
    0.95, // 12
    0.98, // 13
    1.00  // 14
};
static const char corner_array[][4] = {"BET", "CHI", "DLM", "FSU",
                                       "GJZ", "KNW", "PRV"};
static const char edge_array[][3] = {"AM", "BQ", "CE", "DI", "FT", "HJ",
                                     "KZ", "LN", "OW", "PR", "SU"};

const size_t n_edge_array = sizeof(edge_array) / sizeof(*edge_array);
const size_t n_corner_array = sizeof(corner_array) / sizeof(*corner_array);

void printColChar(char c, enum Color col) {
  switch (col) {
  case Red:
    printf("\033[31m");
    break;
  case Green:
    printf("\033[32m");
    break;
  default:;
  }
  printf("%c\033[0m", c);
}

void clearScreen(void) { printf("\033[1;1H\033[2J"); }

Cube *_scramble_cube(void) {
  // begin setup

  float f;
  uint8_t n_corners = 0;
  uint8_t n_edges = 0;
  uint8_t j = 0;
  f = (float)rand() / RAND_MAX;
  for (; j < max_c - min_c; j++) {
    if (corner_cdf[j] >= f) {
      n_corners = j + min_c;
      break;
    }
  }
  f = (float)rand() / RAND_MAX;
  j = 0;
  for (; j < max_e - min_e; j++) {
    if (edge_cdf[j] >= f) {
      if ((n_corners % 2 == 1 && j % 2 == 0) ||
          (n_corners % 2 == 0 && j % 2 == 1)) {
        n_edges = j + 1 + min_e;
      } else {
        n_edges = j + min_e;
      }
    }
  }

  // begin allocation
  Cube *cube;
  if ((cube = malloc(sizeof(Cube))) == NULL) {
    return NULL;
  }
  if ((cube->corners = calloc(n_corners + 1, sizeof(char))) == NULL) {
    free(cube);
    return NULL;
  }
  if ((cube->edges = calloc(n_edges + 1, sizeof(char))) == NULL) {
    free(cube->corners);
    free(cube);
    return NULL;
  }

  // begin assignment, naive implementation for now
  uint8_t prev_idx = -1;
  uint8_t curr_idx = -1;
  uint8_t letter_idx;
  for (uint8_t i = 0; i < n_edges; i++) {
    do {
      curr_idx = (float)rand() / RAND_MAX * n_edge_array;
    } while (curr_idx == prev_idx);
    letter_idx = (float)rand() / RAND_MAX * 2;
    cube->edges[i] = edge_array[curr_idx][letter_idx];
    prev_idx = curr_idx;
  }
  prev_idx = -1;
  curr_idx = -1;
  for (uint8_t i = 0; i < n_corners; i++) {
    do {
      curr_idx = (float)rand() / RAND_MAX * n_corner_array;
    } while (curr_idx == prev_idx);
    letter_idx = (float)rand() / RAND_MAX * 3;
    cube->corners[i] = corner_array[curr_idx][letter_idx];
    prev_idx = curr_idx;
  }

  return cube;
}

void _free_cube(Cube *c) {
  free(c->edges);
  free(c->corners);
  free(c);
}

Cubes *scramble(uint32_t n) {
  Cubes *cubes;
  if ((cubes = malloc(sizeof(Cubes))) == NULL) {
    return NULL;
  }
  if ((cubes->data = malloc(sizeof(Cube) * n)) == NULL) {
    free(cubes);
    return NULL;
  }
  cubes->n = n;
  for (uint32_t i = 0; i < n; i++) {
    Cube *cube;
    if ((cube = _scramble_cube()) == NULL) {
      for (uint32_t j = 0; j < i; j++) {
        _free_cube(cubes->data[j]);
      }
      free(cubes->data);
      free(cubes);
    }
    cubes->data[i] = cube;
  }
  return cubes;
}

void freeCubes(Cubes *ptr) {
  for (size_t i = 0; i < ptr->n; i++) {
    _free_cube(ptr->data[i]);
  }
  free(ptr->data);
  free(ptr);
}

void _displayEdges(Cube *c) {
  int32_t sz = strlen(c->edges);
  if (sz % 2 == 0) {
    for (uint32_t j = 0; j < sz; j += 2) {
      printf("%c%c ", c->edges[j], c->edges[j + 1]);
    }
  } else {
    for (uint32_t j = 0; j < sz - 1; j += 2) {
      printf("%c%c ", c->edges[j], c->edges[j + 1]);
    }
    printf("%c", c->edges[sz - 1]);
  }
}

void _displayCorners(Cube *c) {
  int32_t sz = strlen(c->corners);
  if (sz % 2 == 0) {
    for (uint32_t j = 0; j < sz; j += 2) {
      printf("%c%c ", c->corners[j], c->corners[j + 1]);
    }
  } else {
    sz = strlen(c->corners);
    for (uint32_t j = 0; j < sz - 1; j += 2) {
      printf("%c%c ", c->corners[j], c->corners[j + 1]);
    }
    printf("%c", c->corners[sz - 1]);
  }
}

void _displayCube(Cubes *c, uint32_t i) {
  printf(" %d/%d   [s: solve, b/k: back, n/j: next, e: jump, Esc: end]", i + 1,
         c->n);
  printf("\n------\n\n");
  printf("E: ");
  _displayEdges(c->data[i]);
  printf("\nC: ");
  _displayCorners(c->data[i]);
  printf("\n"); // so cursor is on the line below
}

char input_in_edges(char c) {
  for (uint8_t i = 0; i < n_edge_array; i++) {
    if (edge_array[i][0] == c || edge_array[i][0] == c - 32) {
      return edge_array[i][0];
    } else if (edge_array[i][1] == c || edge_array[i][1] == c - 32) {
      return edge_array[i][1];
    }
  }
  return 0;
}

char input_in_corners(char c) {
  for (uint8_t i = 0; i < n_corner_array; i++) {
    if (corner_array[i][0] == c || corner_array[i][0] == c - 32) {
      return corner_array[i][0];
    } else if (corner_array[i][1] == c || corner_array[i][1] == c - 32) {
      return corner_array[i][1];
    } else if (corner_array[i][2] == c || corner_array[i][2] == c - 32) {
      return corner_array[i][2];
    }
  }
  return 0;
}

bool isGameStateMemoing(GameState *gs) { return (gs->flags & 1) == 0; }

bool isGameStateSolving(GameState *gs) { return (gs->flags & 1) == 1; }

bool isGameStateExecuting(GameState *gs) { return ((gs->flags >> 1) & 1) == 1; }
void setGameStateExecuting(GameState *gs) {
  // note that this also sets solving to true
  gs->flags = gs->flags | ~(~(uint32_t)1 << 1);
}

void setGameStateMemoing(GameState *gs) {
  gs->flags = gs->flags & ~((uint32_t)1 << 0);
}

void setGameStateSolving(GameState *gs) { gs->flags = gs->flags | 1; }

GameState *allocateGameState(Cubes *cubes) {
  GameState *gs;
  struct termios old_settings, new_settings;
  if ((gs = malloc(sizeof(GameState))) == NULL) {
    return NULL;
  }
  // Get current terminal settings
  tcgetattr(STDIN_FILENO, &old_settings);
  // Modify settings for non-echoing input
  new_settings = old_settings;
  new_settings.c_lflag &=
      ~(ICANON | ECHO); // Disable canonical mode and echoing
  tcsetattr(STDIN_FILENO, TCSANOW, &new_settings);

  gs->cubes = cubes;
  gs->flags = 0;
  gs->screen_idx = 0;
  gs->old_settings = old_settings;
  gs->new_settings = new_settings;
  return gs;
}

void deallocateGameState(GameState *gs) {
  tcsetattr(STDIN_FILENO, TCSANOW, &gs->old_settings);
  free(gs);
}

void displayGame(GameState *gs) {
  if (isGameStateMemoing(gs)) {
    _displayCube(gs->cubes, gs->screen_idx);
  } else if (isGameStateExecuting(gs)) {
    // is solving
    char c;
    Cubes *cubes = gs->cubes;
    uint32_t n_cubes = cubes->n;
    char entries[2 * n_cubes][32];
    for (size_t i = 0; i < 2 * n_cubes; i++) {
      memset(entries[i], 0, 32);
    }
    for (uint32_t i = 0; i < n_cubes; i++) {
      bool on_edges = true; // note that when this is false, on_corners == true
      uint8_t idx = 0;
      char toprint;
      printf("%d/%d", i + 1, n_cubes);
      printf("\n------\n\n");
      printf("E: ");
      while ((c = getchar()) != 27) {
        if (on_edges) {
          if ((toprint = input_in_edges(c))) {
            if (idx % 2 == 0 && idx != 0) {
              printf(" ");
            }
            printf("%c", toprint);
            entries[2 * i][idx % 31] = toprint;
            idx += 1;
          } else if (c == '\n') {
            on_edges = false;
            idx = 0;
            printf("\nC: ");
          }
        } else {
          if ((toprint = input_in_corners(c))) {
            if (idx % 2 == 0 && idx != 0) {
              printf(" ");
            }
            entries[2 * i + 1][idx % 31] = toprint;
            printf("%c", toprint);
            idx += 1;
          } else if (c == '\n') {
            on_edges = true;
            printf("\n------\n\n");
            break;
          }
        }
      }
    }
    // done "solving", now we show the result!
    clearScreen();
    for (uint32_t i = 0; i < n_cubes; i++) {
      Cube *cube = cubes->data[i];
      size_t sz_edges = strlen(entries[2 * i]);
      size_t sz_corns = strlen(entries[2 * i + 1]);
      size_t sz_edges_src = strlen(cube->edges);
      size_t sz_corns_src = strlen(cube->corners);
      printf("------\n");
      printf("%d/%d", i + 1, n_cubes);
      printf("\n------\n\n");
      printf("E: \n");
      _displayEdges(cube);
      printf("\n");
      for (size_t j = 0; j < sz_edges; j++) {
        if (j % 2 == 0 && j != 0) {
          printf(" ");
        }
        if (j >= sz_edges_src || cube->edges[j] != entries[2 * i][j]) {
          printColChar(entries[2 * i][j], Red);
        } else {
          printColChar(entries[2 * i][j], Green);
        }
      }
      printf("\n\nC: \n");
      _displayCorners(cube);
      printf("\n");
      for (size_t j = 0; j < sz_corns; j++) {
        if (j % 2 == 0 && j != 0) {
          printf(" ");
        }
        if (j >= sz_corns_src || cube->corners[j] != entries[2 * i + 1][j]) {
          printColChar(entries[2 * i + 1][j], Red);
        } else {
          printColChar(entries[2 * i + 1][j], Green);
        }
      }
      printf("\n\n");
    }
  } 
  else {
    printf("Goodbye.\n");
  }
}

void incrementGameStateScreen(GameState *gs) {
  gs->screen_idx = (gs->screen_idx + 1) % gs->cubes->n;
}

void decrementGameStateScreen(GameState *gs) {
  uint32_t n_cubes = gs->cubes->n;
  if (gs->screen_idx == 0) {
    gs->screen_idx = n_cubes - 1;
  } else {
    gs->screen_idx = (gs->screen_idx - 1) % n_cubes;
  }
}

void zeroGameStateScreen(GameState *gs) { gs->screen_idx = 0; }

bool game(uint32_t n_cubes) {
  Cubes *cubes = scramble(n_cubes);
  GameState *state = allocateGameState(cubes);
  char c;
  displayGame(state);
  while (isGameStateMemoing(state) && (c = getchar()) != 27) {
    switch (c) {
    case '\n':
    case 'N':
    case ' ':
    case 'j':
    case 'n':
      incrementGameStateScreen(state);
      clearScreen();
      break;
    case 8:
    case 127:
    case 'B':
    case 'b':
    case 'k':
      decrementGameStateScreen(state);
      clearScreen();
      break;
    case 'S':
    case 's':
      // we are now solving, this breaks out of the while loop
      setGameStateExecuting(state);
    case 'R':
    case 'r':
      zeroGameStateScreen(state);
      clearScreen();
      break;
    default:
      clearScreen();
    }
    if (isGameStateMemoing(state)) {
      displayGame(state);
    }
  }
  setGameStateSolving(state);
  clearScreen();
  displayGame(state); // may lead to exec step
  deallocateGameState(state);
  freeCubes(cubes);
  return 0;
}
