#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"
#include "kernel/fcntl.h"

#define MAXLINE 256
#define MAXARGS 10

int main(int argc, char *argv[])
{
    char line[MAXLINE];
    char *args[MAXARGS];
    int n;

    /*
     * Main function of the xargs program.
     * It takes command-line arguments and reads lines from standard input,
     * executing the specified command with each line as an argument.
     */

    // Check if a command is provided
    if (argc < 2)
    {
        fprintf(2, "Usage: xargs command [args...]\n");
        exit(1);
    }

    /*
     * Store the command and its arguments.
     * The first argument is the command to execute, followed by any additional arguments.
     */
    for (n = 1; n < argc; n++)
    {
        args[n - 1] = argv[n];
    }
    args[n - 1] = 0; // NULL-terminate the arguments

    // Read lines from standard input
    while (1)
    {
        int i = 0;
        while (i < MAXLINE - 1)
        {
            char c;
            /*
             * Read one character from standard input.
             * If the read fails and no characters have been read, exit.
             */
            if (read(0, &c, 1) != 1)
            {
                // Check for EOF or error
                if (i == 0)
                {
                    return 0; // Exit if no input
                }
                break; // End of input
            }
            if (c == '\n')
            {
                break; // End of line
            }
            line[i++] = c; // Store character
        }
        line[i] = 0; // Null-terminate the line

        /*
         * Remove quotes from the start and end of the line if they exist.
         * This is done to clean up the input before processing.
         */
        if (i > 0 && line[0] == '"' && line[i - 1] == '"')
        {
            // Shift line to remove leading quote
            for (int j = 1; j < i - 1; j++)
            {
                line[j - 1] = line[j];
            }
            line[i - 2] = 0; // Null-terminate the modified line
            i -= 2;          // Update length
        }

        // If there's a line, fork and execute the command
        if (i > 0)
        {
            /*
             * Create a new argument list for execution.
             * Combine the original command arguments with the current input line.
             */
            char *exec_args[MAXARGS + 1];
            for (int j = 0; j < n - 1; j++)
            {
                exec_args[j] = args[j]; // Copy command args
            }
            exec_args[n - 1] = line; // Add the line as the last argument
            exec_args[n] = 0;        // Null-terminate for exec

            int pid = fork();
            if (pid < 0)
            {
                fprintf(2, "Fork failed\n");
                exit(1);
            }
            if (pid == 0)
            {                                  // Child process
                exec(exec_args[0], exec_args); // Execute command
                fprintf(2, "Exec failed\n");
                exit(1); // Exit if execution fails
            }

            // Parent process: wait for the child
            int status;    // Variable to hold the exit status of the child
            wait(&status); // Pass the address of status
        }
    }

    exit(0);
}
