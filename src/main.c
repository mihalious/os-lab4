#define _POSIX_C_SOURCE 199309L

#include <errno.h>
#include <fcntl.h>
#include <semaphore.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

#define LEN 64
int main() {
    FILE *fptr = fopen("./pid", "r");
    if (fptr == nullptr) {
        printf("Couldn't get process id\n");
        return 1;
    }
    char pid_buf[LEN] = {};
    char *pid_str = fgets(pid_buf, LEN, fptr);
    printf("pid: %s\n", pid_str);
    char addr_buf[LEN] = {};
    char *addr_str = fgets(addr_buf, LEN, fptr);
    printf("addr: %s\n", addr_str);
    fclose(fptr);

    uint32_t pid = 0;
    sscanf(pid_str, "%d", &pid);
    if (pid == 0) {
        printf("Couldn't parse process id\n");
        return 1;
    }
    printf("pid: %d\n", pid);

    size_t addr = 0;
    sscanf(addr_str, "%lx", &addr);
    if (addr == 0) {
        printf("Couldn't parse address of variable\n");
        return 1;
    }
    printf("address: %ld\n", addr);

    char file_name[LEN] = {};
    int32_t err = sprintf(file_name, "/proc/%d/mem", pid);
    if (err < 0) {
        printf("Couldn't construct /proc/%d/mem filename\n", pid);
        return 1;
    }
    printf("pid: %s\n", file_name);

    int32_t fd = open(file_name, O_RDONLY);
    if (fd == -1) {
        printf("Couldn't open /proc/%d/mem file\n", pid);
        if (errno == EACCES) {
            printf("Acces denied\n");
        }
        return 1;
    }
    printf("fd: %d\n", fd);

    sem_t *semp = sem_open("/writer_semaphore", 0, 0666, 0);
    if (semp == SEM_FAILED) {
        printf("Failed to open semphore");
        return 1;
    }

    char c = {};
    lseek(fd, addr, SEEK_SET);
    while (true) {
        sem_wait(semp);
        ssize_t bytes_read = read(fd, &c, 1);
        printf("Bytes read: %ld\n", bytes_read);
        if (bytes_read == -1) {
            printf("Couldn't read from file\n");
            return 1;
        }
        printf("Read value of c in proccess: %c (%u)\n", c, c);
    }
    close(fd);
}
