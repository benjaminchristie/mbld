#include "mbld.h"
#include <time.h>

int main(int argc, char** argv) {
    srand(time(0));
    if (argc != 2) {
        printf("Must be called with two parameters as follows: ./main $n where $n is the number of cubes to solve.\n");
        return 1;
    }
    clearScreen();
    uint32_t n_cubes = atoi(argv[1]);
    if (n_cubes == 0 || n_cubes > 100000) {
        printf("$n must be in the range [1, 100000].\n");
        return 2;
    }
    game(n_cubes);
    return 0;
}
