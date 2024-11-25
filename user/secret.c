#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(2, "Usage: secret <8-byte-string>\n");
        exit(1);
    }

    // Allocate a page of memory
    char *mem = sbrk(4096);
    if (mem == (char *)-1)
    {
        fprintf(2, "sbrk failed\n");
        exit(1);
    }

    // Copy secret to memory at offset 32
    char *secret_loc = mem + 32;
    memmove(secret_loc, argv[1], 8);

    // Hold the memory briefly
    sleep(1);

    exit(0);
}