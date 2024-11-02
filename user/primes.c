/**
 * @file primes.c
 * @author Abdelrahman Amr Elsayed (abdelrahman.elsayed@ejust.edu.eg) , ID : 120210128
 * @brief 
 * @version 0.1
 * @date 2024-10-26
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "kernel/types.h"
#include "user/user.h"

#define MAX_NUM 280
#define READ 0
#define WRITE 1
#define INT_SIZE 4

// Forward declaration with noreturn
int new_proc(int left[]) __attribute__((noreturn));

// Function to create a new process and filter primes
int new_proc(int left[])
{
  int prime, temp, pid;
  int right[2];

  close(left[WRITE]);  // Close the write end of the pipe for reading

  // Read the first prime number
  if (read(left[READ], &prime, INT_SIZE) <= 0) {
    close(left[READ]);
    exit(0);  // Exit if no prime is found
  }

  printf("prime %d\n", prime);

  // Create a new pipe for the next process
  pipe(right);

  if ((pid = fork()) < 0) {  // Fork error
    close(right[WRITE]);
    close(right[READ]);
    close(left[READ]);
    fprintf(1, "primes: fork failed\n");
    exit(-1);
  } else if (pid > 0) {  // Parent process
    close(right[READ]);  // Close unused read end in the parent

    // Filter and write non-multiples of the prime to the next pipe
    while (read(left[READ], &temp, INT_SIZE) > 0) {
      if (temp % prime != 0) {
        write(right[WRITE], &temp, INT_SIZE);
      }
    }

    close(right[WRITE]);  // Close the write end of the right pipe
    close(left[READ]);    // Close the read end of the left pipe

    wait(0);  // Wait for the child process to finish
    exit(0);
  } else {  // Child process
    close(left[READ]);  // Close the left pipe completely for the child
    new_proc(right);    // Recursively create new processes
  }
}

int main(int argc, char* argv[])
{
  int p[2], pid;
  pipe(p);  // Create the initial pipe

  if ((pid = fork()) < 0) {  // Fork error handling
    close(p[WRITE]);
    close(p[READ]);
    exit(1);
  } else if (pid > 0) {  // Parent process
    close(p[READ]);  // Parent only writes to the pipe

    // Write numbers from 2 to MAX_NUM to the pipe
    for (int i = 2; i <= MAX_NUM; i++) {
      write(p[WRITE], &i, INT_SIZE);
    }

    close(p[WRITE]);  // Close the write end after sending all numbers
    wait(0);          // Wait for the child process to finish
    exit(0);
  } else {  // Child process
    close(p[WRITE]);  // Child only reads from the pipe
    new_proc(p);      // Start the prime filtering process
  }

  return 0;
}
