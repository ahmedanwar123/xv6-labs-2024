#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    int p1[2]; // Pipe: parent writes, child reads
    int p2[2]; // Pipe: child writes, parent reads
    char recv_buf[5]; // Buffer for received messages
    int status;

    pipe(p1); // Create pipe for parent -> child
    pipe(p2); // Create pipe for child -> parent

    if (fork() == 0) {
        // --- Child process ---
        close(p1[1]); // Close unused write end of p1
        read(p1[0], recv_buf, 4); // Read "ping"
        recv_buf[4] = '\0';
        close(p1[0]);

        printf("%d: received %s\n", getpid(), recv_buf);

        close(p2[0]); // Close unused read end of p2
        write(p2[1], "pong", 4); // Send response
        close(p2[1]);

        exit(0);
    } else {
        // --- Parent process ---
        close(p1[0]); // Close unused read end of p1
        write(p1[1], "ping", 4); // Send message
        close(p1[1]);

        close(p2[1]); // Close unused write end of p2
        read(p2[0], recv_buf, 4); // Read response
        recv_buf[4] = '\0';
        close(p2[0]);

        printf("%d: received %s\n", getpid(), recv_buf);

        wait(&status);
        exit(0);
    }
}
