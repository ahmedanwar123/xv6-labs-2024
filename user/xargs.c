#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

int main(int argc, char *argv[]) {
    char buf[512];
    char *args[MAXARG];
    int i, n;
    int pid;

    if (argc < 2) {
        fprintf(2, "Usage: xargs <command>...\n");
        exit(1);
    }

    for (i = 0; i < argc - 1; i++) {
        args[i] = argv[i + 1];
    }

    while (1) {
        n = 0;
        while (n < sizeof(buf) - 1) {
            if (read(0, buf + n, 1) != 1) break;
            if (buf[n] == '\n') {
                buf[n] = 0;
                break;
            }
            n++;
        }
        
        if (n == 0) break;

        args[argc - 1] = buf;

        if ((pid = fork()) == 0) {
            exec(args[0], args);
            fprintf(2, "xargs: exec %s failed\n", args[0]);
            exit(1);
        }
        
        wait(0);
    }

    exit(0);
}
