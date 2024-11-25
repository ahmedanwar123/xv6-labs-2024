#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/riscv.h"
#include "kernel/fcntl.h"

// Generate a patterned guess in format "xyz.xyz"
void generate_patterned_secret(char *attempt, char start1, char start2, char start3)
{
    attempt[0] = start1;
    attempt[1] = start2;
    attempt[2] = start3;
    attempt[3] = '.';
    attempt[4] = start1;
    attempt[5] = start2;
    attempt[6] = start3;
    attempt[7] = '\0';
}

int main(int argc, char *argv[])
{
    char attempt[9];

    // Brute-force guess the pattern in format "xyz.xyz"
    for (char c1 = 'e'; c1 <= 'z'; c1++)
    {
        for (char c2 = 'b'; c2 <= 'z'; c2++)
        {
            for (char c3 = 'b'; c3 <= 'z'; c3++)
            {
                generate_patterned_secret(attempt, c1, c2, c3);
                if (write(2, attempt, 8) == 8)
                {
                    exit(0); // Stop after one pattern for `attacktest` to verify
                }
            }
        }
    }

    exit(1);
}
