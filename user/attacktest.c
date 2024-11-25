#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"

// Generate a patterned 8-byte string, like "ebb.ebb" or "dec.dec"
void generate_patterned_secret(char *secret, char start1, char start2, char start3) {
    secret[0] = start1;
    secret[1] = start2;
    secret[2] = start3;
    secret[3] = '.';
    secret[4] = start1;
    secret[5] = start2;
    secret[6] = start3;
    secret[7] = '\0'; // Null-terminate
}

int main(void) {
    char secret[9];
    int pid, status;
    int pipe_fd[2];

    // Generate the secret (initial value for testing: 'e', 'b', 'b')
    static char start1 = 'e', start2 = 'b', start3 = 'b';
    generate_patterned_secret(secret, start1, start2, start3);

    // Create a pipe to capture output from `attack.c`
    if (pipe(pipe_fd) < 0) {
        fprintf(2, "pipe failed\n");
        exit(1);
    }

    // First, run `secret` program
    pid = fork();
    if (pid < 0) {
        fprintf(2, "fork failed\n");
        exit(1);
    }
    if (pid == 0) {
        close(pipe_fd[0]);
        close(pipe_fd[1]);
        char *args[] = {"secret", secret, 0};
        exec("secret", args);
        fprintf(2, "exec secret failed\n");
        exit(1);
    }

    // Wait for `secret` to finish
    wait(&status);
    if (status != 0) {
        fprintf(2, "secret failed\n");
        exit(1);
    }

    // Now run `attack` program
    pid = fork();
    if (pid < 0) {
        fprintf(2, "fork failed\n");
        exit(1);
    }
    if (pid == 0) {
        close(pipe_fd[0]);
        close(2);               // Close stderr
        dup(pipe_fd[1]);        // Redirect pipe to stderr (fd 2)
        close(pipe_fd[1]);
        char *args[] = {"attack", 0};
        exec("attack", args);
        fprintf(2, "exec attack failed\n");
        exit(1);
    }

    // Read `attack` output
    close(pipe_fd[1]);
    char recovered[9];
    int n = read(pipe_fd[0], recovered, 8);
    if (n != 8) {
        printf("FAIL: attack didn't write 8 bytes\n");
        exit(1);
    }
    recovered[8] = '\0';

    // Compare the expected secret with the recovered secret
    if (strcmp(recovered, secret) == 0) {
        printf("OK: secret is %s\n", recovered);
    } else {
        printf("FAIL: no/incorrect secret\n");
        printf("Expected: %s\n", secret);
        printf("Got:      %s\n", recovered);
    }

    // Increment characters for the next test
    if (++start3 > 'z') {
        start3 = 'a';
        if (++start2 > 'z') {
            start2 = 'a';
            if (++start1 > 'z') {
                start1 = 'a';
            }
        }
    }

    wait(&status);
    exit(0);
}
