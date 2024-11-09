#include "kernel/types.h"
#include "user/user.h"

int prime_filter(int pipe_input[]) __attribute__((noreturn));

int prime_filter(int pipe_input[])
{
    int prime, number, pid;
    int pipe_output[2];

    close(pipe_input[1]);

    if (read(pipe_input[0], &prime, 4) <= 0)
    {
        close(pipe_input[0]);
        exit(0);
    }

    printf("prime %d\n", prime);

    pipe(pipe_output);

    if ((pid = fork()) < 0)
    {
        close(pipe_output[1]);
        close(pipe_output[0]);
        close(pipe_input[0]);
        fprintf(1, "primes: fork failed\n");
        exit(-1);
    }
    else if (pid > 0)
    {
        close(pipe_output[0]);

        while (read(pipe_input[0], &number, 4) > 0)
        {
            if (number % prime != 0)
            {
                write(pipe_output[1], &number, 4);
            }
        }

        close(pipe_output[1]);
        close(pipe_input[0]);

        wait(0);
        exit(0);
    }
    else
    {
        close(pipe_input[0]);
        prime_filter(pipe_output);
    }
}

int main(int argc, char *argv[])
{
    int pipe_fd[2], pid;
    pipe(pipe_fd);

    if ((pid = fork()) < 0)
    {
        close(pipe_fd[1]);
        close(pipe_fd[0]);
        exit(1);
    }
    else if (pid > 0)
    {
        close(pipe_fd[0]);

        for (int i = 2; i <= 290; i++)
        {
            write(pipe_fd[1], &i, 4);
        }

        close(pipe_fd[1]);
        wait(0);
        exit(0);
    }
    else
    {
        close(pipe_fd[1]);
        prime_filter(pipe_fd);
    }

    return 0;
}

