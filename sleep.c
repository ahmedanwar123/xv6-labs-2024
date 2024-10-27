/*
Name: Ahmed Anwar Mazhar
ID: 120210007
*/
#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    int sleep_sec;
    if (argc < 2)
    {
        printf("Usage: sleep seconds\n");
        exit(1);
    }

    sleep_sec = atoi(argv[1]);
    if (sleep_sec > 0)
    {
        sleep(sleep_sec);
    }
    else
    {
        printf("Invalid interval %s\n", argv[1]);
        exit(1);
    }

    exit(0);
}

