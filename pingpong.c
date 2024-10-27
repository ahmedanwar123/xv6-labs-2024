#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    int p[2];              // File descriptors for the pipe
    char recv_buf[5];     // Buffer to store received data (+1 for null terminator)
    int status;            // Variable to hold exit status

    pipe(p);              // Create a pipe

    if (fork() == 0) {    // Child process
        close(p[1]);      // Close the write end of the pipe in the child
        read(p[0], recv_buf, 4);  // Read data from the pipe
        recv_buf[4] = '\0'; // Null-terminate the string
        close(p[0]);      // Close the read end of the pipe
        printf("%d: received %s\n", getpid(), recv_buf); // Print the received message
        write(p[1], "pong", 4);  // Write response back to the pipe
        close(p[1]);      // Close the write end of the pipe
        exit(0);          // Exit child process with status 0
    } else {              // Parent process
        close(p[0]);      // Close the read end of the pipe in the parent
        write(p[1], "ping", 4);  // Write message to child
        close(p[1]);      // Close the write end of the pipe
        read(p[0], recv_buf, 4); // Read response from child
        recv_buf[4] = '\0'; // Null-terminate the string
        close(p[0]);      // Close the read end of the pipe
        printf("%d: received %s\n", getpid(), recv_buf); // Print the received message
        wait(&status);    // Wait for child process to finish and retrieve status
        exit(0);          // Exit parent process with status 0
    }
}

