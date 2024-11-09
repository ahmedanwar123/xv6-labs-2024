#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: sleep <ticks>\n");
        exit(1);  // Exit with status 1 to indicate an error
    }

    int ticks = atoi(argv[1]);
    sleep(ticks);
    exit(0);  // Normal exit
}

