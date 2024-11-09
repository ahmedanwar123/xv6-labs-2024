#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    int pipe_fd[2];      
    char buffer[2];      

    if (pipe(pipe_fd) < 0) {
        printf("Failed to create pipe\n");
        exit(1);         
    }

    if (fork() == 0) {   
        close(pipe_fd[1]); 
        read(pipe_fd[0], buffer, 1); 
        printf("%d: received ping\n", getpid());
        write(pipe_fd[1], "p", 1);  
        close(pipe_fd[0]);
        exit(0);         
    } else {
        close(pipe_fd[0]); 
        write(pipe_fd[1], "i", 1);  
        close(pipe_fd[1]); 

        wait((int*)0); 
        read(pipe_fd[0], buffer, 1);
        printf("%d: received pong\n", getpid());
        close(pipe_fd[0]);
        exit(0);        
    }
}
