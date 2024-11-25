#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"

// Generate a random 8-byte string
void generate_secret(char *secret)
{
    // Simple PRNG for demonstration
    static unsigned int next = 1;
    for (int i = 0; i < 8; i++)
    {
        next = next * 1103515245 + 12345;
        secret[i] = (char)((next / 65536) % 256);
    }
}

int main(void)
{
    char secret[9]; // 8 bytes + null terminator
    int pid, status;
    int pipe_fd[2];

    // Generate the secret
    generate_secret(secret);
    secret[8] = 0; // null terminate

    printf("Generated secret: ");
    for (int i = 0; i < 8; i++)
    {
        printf("%02x", (unsigned char)secret[i]);
    }
    printf("\n");

    // Create pipe for receiving attack output
    if (pipe(pipe_fd) < 0)
    {
        fprintf(2, "pipe failed\n");
        exit(1);
    }

    // First run secret program
    pid = fork();
    if (pid < 0)
    {
        fprintf(2, "fork failed\n");
        exit(1);
    }
    if (pid == 0)
    {
        close(pipe_fd[0]);
        close(pipe_fd[1]);
        char *args[] = {"secret", secret, 0};
        exec("secret", args);
        fprintf(2, "exec secret failed\n");
        exit(1);
    }

    // Wait for secret to finish
    wait(&status);
    if (status != 0)
    {
        fprintf(2, "secret failed\n");
        exit(1);
    }

    // Now run attack program
    pid = fork();
    if (pid < 0)
    {
        fprintf(2, "fork failed\n");
        exit(1);
    }
    if (pid == 0)
    {
        close(pipe_fd[0]);
        // Redirect stderr to pipe using close and dup
        close(2);        // Close stderr
        dup(pipe_fd[1]); // Duplicate pipe write end to stderr (fd 2)
        close(pipe_fd[1]);
        char *args[] = {"attack", 0};
        exec("attack", args);
        fprintf(2, "exec attack failed\n");
        exit(1);
    }

    // Read attack output
    close(pipe_fd[1]);
    char recovered[9];
    int n = read(pipe_fd[0], recovered, 8);
    if (n != 8)
    {
        printf("FAIL: attack didn't write 8 bytes\n");
        exit(1);
    }
    recovered[8] = 0;

    // Compare results
    int match = 1;
    for (int i = 0; i < 8; i++)
    {
        if (recovered[i] != secret[i])
            match = 0;
    }

    if (match)
    {
        printf("OK: secret is ");
        for (int i = 0; i < 8; i++)
        {
            printf("%02x", (unsigned char)recovered[i]);
        }
        printf("\n");
    }
    else
    {
        printf("FAIL: no/incorrect secret\n");
        printf("Expected: ");
        for (int i = 0; i < 8; i++)
        {
            printf("%02x", (unsigned char)secret[i]);
        }
        printf("\nGot:      ");
        for (int i = 0; i < 8; i++)
        {
            printf("%02x", (unsigned char)recovered[i]);
        }
        printf("\n");
    }

    wait(&status);
    exit(0);
}