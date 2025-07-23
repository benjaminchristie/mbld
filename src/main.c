#include "mbld.h"
#include <time.h>

int main(int argc, char** argv) {
    srand(time(0));
    if (argc != 2) {
        printf("Must be called with two parameters as follows: ./main $n where $n is the number of cubes to solve.\n");
        return 1;
    }
    clearScreen();
    game(atoi(argv[1]));
    return 0;
}
