#define _POSIX_C_SOURCE 199309L

#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>

void milisleep(uint64_t miliseconds) {
    struct timespec ts = {.tv_sec = (miliseconds / 1000),
                          .tv_nsec = (miliseconds % 1000) * 1000000};

    nanosleep(&ts, nullptr);
}

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

    // const size_t len = 1'000'000;
    // char *proc_mem_buf = calloc(len, sizeof(char));
    // if (!proc_mem_buf) {
    //     close(fd);
    //     return 1;
    // }
    // off_t offset = lseek(fd, 0x7ffc9cc17000, SEEK_SET);
    // ssize_t bytes_read =
    //     read(fd, proc_mem_buf, 0x7ffc9cc38000 - 0x7ffc9cc17000);
    // if (bytes_read == -1) {
    //     printf("Couldn't read from file\n");
    //     return 1;
    // }

    // printf("Read %ld bytes form /proc/%d/mem\n", bytes_read, pid);
    // for (ssize_t i = 0; i < bytes_read; i++) {
    //     printf("%u", proc_mem_buf[bytes_read - i]);
    // }
    // printf("\n");
    //  printf("%s", proc_mem_buf);
    // free(proc_mem_buf);

    lseek(fd, addr, SEEK_SET);
    char c = {};
    while (true) {
        char tmp = {};
        ssize_t bytes_read = read(fd, &tmp, 1);
        if (bytes_read == -1) {
            printf("Couldn't read from file\n");
            return 1;
        }
        if (tmp != c){
            c = tmp;
            printf("Read value of c in proccess: %c (%u)\n", c, c);
        }
        lseek(fd, addr, SEEK_SET);
        //milisleep(20);
        //printf("c form proccess: %u\n", c);
    }
    close(fd);
}
