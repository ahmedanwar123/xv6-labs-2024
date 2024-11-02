#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"

void execute_line(char *line, char *args[], int skip) {
    int ws = 1;
    char **a, *l;

    // Split input line into words based on whitespace
    a = &args[skip];
    l = line;
    while (*l != '\0') {
        if (*l == ' ') {
            if (!ws) {
                *l = '\0';
                a++;
            }
            ws = 1;
        } else {
            if (ws) {
                *a = l;
            }
            ws = 0;
        }
        l++;
    }

    int f = fork();
    if (f < 0) {
        fprintf(2, "xargs: fork failed\n");
        exit(1);
    } else if (f == 0) { // child process
        exec(args[0], args);
        // If exec fails, print an error and exit
        fprintf(2, "xargs: exec failed\n");
        exit(1);
    } else { // parent process
        wait((int *)0);
    }
}

int main(int argc, char *argv[]) {
    char *args[MAXARG] = { 0 };
    char buf[512], *b;
    b = buf;

    // Copy command-line arguments into args array for exec
    for (int i = 1; i < argc; i++) {
        args[i - 1] = argv[i];
    }

    // Read each line of input
    while (read(0, b, 1) > 0) {
        if (*b == '\n') {
            *b = '\0';  // Null-terminate the line
            execute_line(buf, args, argc - 1); // Execute command with line arguments
            b = buf; // Reset buffer pointer to start for the next line
        } else {
            b++;
        }
    }

    // Execute final line if not followed by newline
    if (b != buf) {
        *b = '\0';
        execute_line(buf, args, argc - 1);
    }
    
    exit(0);
}
