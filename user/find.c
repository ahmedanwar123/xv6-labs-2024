#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

// Extracts the file name from a given path
char *get_filename(char *path)
{
    char *p;
    for (p = path + strlen(path); p >= path && *p != '/'; p--)
        ; // Find last '/'
    p++;
    return p; // Returns pointer to file name portion
}

// Recursive function to search for a file within directories
void search_directory(char *path, char *targetname)
{
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    // Print the path if it matches the target file name
    if (!strcmp(get_filename(path), targetname))
    {
        printf("%s\n", path);
    }

    // Open the directory; return if it fails
    if ((fd = open(path, O_RDONLY)) < 0)
    {
        fprintf(2, "search_directory: cannot open [%s]\n", path);
        return;
    }

    // Get file status; return if it fails
    if (fstat(fd, &st) < 0)
    {
        fprintf(2, "search_directory: cannot stat %s\n", path);
        close(fd);
        return;
    }

    // Check if the path is a directory
    if (st.type != T_DIR)
    {
        close(fd);
        return;
    }

    // Prepare buffer for directory path
    if (strlen(path) + 1 + DIRSIZ + 1 > sizeof(buf))
    {
        printf("search_directory: path too long\n");
        close(fd);
        return;
    }
    strcpy(buf, path);
    p = buf + strlen(buf);
    *p++ = '/';

    // Read directory entries
    while (read(fd, &de, sizeof(de)) == sizeof(de))
    {
        if (de.inum == 0 || !strcmp(de.name, ".") || !strcmp(de.name, ".."))
            continue;

        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;

        // Recursively search in subdirectories
        search_directory(buf, targetname);
    }
    close(fd);
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        fprintf(2, "usage: search_directory path file\n");
        exit(1);
    }

    search_directory(argv[1], argv[2]); // Call the function to search
    exit(0);
}
