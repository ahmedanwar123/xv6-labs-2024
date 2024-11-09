#include "user/user.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: trace <syscall_number> <command> <args>\n");
        exit(1);
    }

    int syscall_num = atoi(argv[1]);
    if (trace(syscall_num) < 0) {
        printf("Failed to set trace mask\n");
        exit(1);
    }

    exec(argv[2], argv + 2);
    return 0;
}

