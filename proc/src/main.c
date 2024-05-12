#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#define LEN 1024
int main() {
    char c = '\0';
    char buf[LEN] = {};

    pid_t pid = getpid();
    printf("pid: %d\n", pid);
    FILE *fd = fopen("../pid", "w");
    if (fd == nullptr) {
        return -1;
    }
    fprintf(fd, "%d\n", pid);
    fprintf(fd, "%p\n", &c);
    fclose(fd);

    fprintf(stderr, "address of c is:    %p\n", &c);
    for (size_t i = 0; true; i++) {
        char tmp = getchar();
        if (tmp == EOF) {
            break;
        }
        if (tmp == '\n') {
            buf[i % LEN] = tmp;
            continue;
        }
        c = tmp;
        buf[i % LEN] = tmp;
        printf("current value of c: %c (%u)\n", c, c);
    }
    printf("Char buffer:\n");
    for (ssize_t i = 0; i < LEN; i++) {
        printf("%c", buf[i]);
    }
}
