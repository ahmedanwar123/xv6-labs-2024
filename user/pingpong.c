/**
 * @file pingpong.c
 * @author Abdelrahman Amr Elsayed (abdelrahman.elsayed@ejust.edu.eg) , ID :120210128
 * @brief 
 * @version 0.1
 * @date 2024-10-26
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char *argv[]) {
    int parent_to_child[2];  // Pipe for parent to child communication
    int child_to_parent[2];  // Pipe for child to parent communication
    char recv_buf[5];        // Buffer to store received data

    // Create two pipes
    pipe(parent_to_child);
    pipe(child_to_parent);

    if (fork() == 0) {    // Child process
        // Close unused ends of the pipes
        close(parent_to_child[1]);  // Close write end of parent-to-child pipe
        close(child_to_parent[0]);  // Close read end of child-to-parent pipe

        // Read "ping" from the parent
        read(parent_to_child[0], recv_buf, 4);
        recv_buf[4] = '\0';  // Null-terminate the string
        printf("%d: received %s\n", getpid(), recv_buf);

        // Send "pong" to the parent
        write(child_to_parent[1], "pong", 4);

        // Close the remaining pipe ends
        close(parent_to_child[0]);
        close(child_to_parent[1]);

        exit(0);  // Exit child process
    } else {      // Parent process
        // Close unused ends of the pipes
        close(parent_to_child[0]);  // Close read end of parent-to-child pipe
        close(child_to_parent[1]);  // Close write end of child-to-parent pipe

        // Send "ping" to the child
        write(parent_to_child[1], "ping", 4);

        // Read "pong" from the child
        read(child_to_parent[0], recv_buf, 4);
        recv_buf[4] = '\0';  // Null-terminate the string
        printf("%d: received %s\n", getpid(), recv_buf);

        // Close the remaining pipe ends
        close(parent_to_child[1]);
        close(child_to_parent[0]);

        // Wait for the child process to avoid zombie
        wait(0);
        exit(0);  // Exit parent process
    }
}
