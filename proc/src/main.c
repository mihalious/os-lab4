#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

#define LEN 1024
int main() {
    printf("%ld\n", sizeof(char));
    char buf[LEN] = {};
    fprintf(stderr, "address of buf is:    %p\n", buf);

    pid_t pid = getpid();
    printf("pid: %d\n", pid);
    FILE *fd = fopen("../pid", "w");
    if (fd == nullptr) {
        return -1;
    }
    fprintf(fd, "%d\n", pid);
    fprintf(fd, "%p\n", buf);
    fclose(fd);

    sem_t *semp = sem_open("/writer_semaphore", O_CREAT, 0666, 0);
    if (semp == SEM_FAILED) {
        printf("Failed to open semphore");
        return 1;
    }

    for (size_t i = 0; i < LEN; i++) {
        char tmp = getchar();
        if (tmp == EOF) {
            break;
        }
        if (tmp == '\n') {
            i--;
            continue;
        }
        buf[i] = tmp;
        sem_post(semp);
        printf("current value of buf[%ld]: %c (%u)\n", i, buf[i], buf[i]);
    }
    sem_close(semp);
    sem_unlink("/writer_semaphore");

    printf("Char buffer:\n");
    for (ssize_t i = 0; i < LEN; i++) {
        printf("%c", buf[i]);
    }
}
