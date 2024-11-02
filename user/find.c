#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "kernel/fs.h"
#include "user/user.h"

void usage() {
    fprintf(1, "usage: find <path> <expression>\n");
}

char* get_basename(char *path) {
    char *base = path;
    for (char *p = path; *p; p++) {
        if (*p == '/') {
            base = p + 1;
        }
    }
    return base;
}

void find(char *path, char *expression) {
    int fd;
    struct stat st;
    struct dirent de;

    if ((fd = open(path, O_RDONLY)) < 0) {
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    if (fstat(fd, &st) < 0) {
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    if (st.type == T_FILE) {
        if (strcmp(get_basename(path), expression) == 0) {
            fprintf(1, "%s\n", path);
        }
        close(fd);
        return;
    }

    if (st.type == T_DIR) {
        char newpath[512];
        int path_len = strlen(path);

        // Ensure there's enough space in `newpath` for the path + "/" + filename
        if (path_len + 1 + DIRSIZ + 1 > sizeof(newpath)) {
            fprintf(2, "find: path too long\n");
            close(fd);
            return;
        }

        strcpy(newpath, path);
        newpath[path_len] = '/';
        char *p = newpath + path_len + 1;

        while (read(fd, &de, sizeof(de)) == sizeof(de)) {
            if (de.inum == 0 || strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0) {
                continue;
            }
            memmove(p, de.name, DIRSIZ);
            p[DIRSIZ] = 0;
            find(newpath, expression);
        }
    }

    close(fd);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        usage();
        exit(1);
    }

    find(argv[1], argv[2]);
    exit(0);
}
